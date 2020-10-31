//
// Created by pbialas on 05.08.2020.
//
#include "utils.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "glad/glad.h"

namespace xe {
    namespace utils {
        std::string get_gl_description(void) {
            std::stringstream ss;
            auto vendor = glGetString(GL_VENDOR);
            auto renderer = glGetString(GL_RENDERER);
            auto version = glGetString(GL_VERSION);
            auto glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);


            int gl_major_version;
            int gl_minor_version;
            glGetIntegerv(GL_MAJOR_VERSION, &gl_major_version);
            glGetIntegerv(GL_MINOR_VERSION, &gl_minor_version);

            ss << vendor << " " << renderer << " OpenGL " << gl_major_version << "." << gl_minor_version << "\n";
            ss << version << " " << glsl_version;

            return ss.str();
        }

        std::string load_file(const std::string &path) {
            std::ifstream file(path, std::ios::in | std::ios::binary);
            if (file) {
                std::ostringstream contents;
                contents << file.rdbuf();
                file.close();
                return (contents.str());
            }
            return {};
        }

        std::string shader_type(GLenum type) {
            switch (type) {
                case GL_VERTEX_SHADER:
                    return "Vertex";
                case GL_FRAGMENT_SHADER:
                    return "Fragment";
                case GL_GEOMETRY_SHADER:
                    return "Geometry";
            }
            return "Unknown";
        }
    }


    GLuint link_program(GLuint program) {
        glLinkProgram(program);
        GLint link_status;
        glGetProgramiv(program, GL_LINK_STATUS, &link_status);
        if (!link_status) {
            std::cerr << "Error linking program\n";
            GLint max_log_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_log_length);
            return 0;
        }


        return program;
    }

    GLuint create_program(const std::string &vs_path, const std::string &fs_path) {

        auto vs_shader = create_shader_from_file(GL_VERTEX_SHADER, vs_path);
        if (vs_shader == 0)
            return 0;

        auto fs_shader = create_shader_from_file(GL_FRAGMENT_SHADER, fs_path);
        if (fs_shader == 0)
            return 0;

        glReleaseShaderCompiler();

        GLuint program = glCreateProgram();
        if (program == 0) {
            std::cerr << "Error creating program" << "\n";
            glDeleteShader(vs_shader);
            glDeleteShader(fs_shader);
            return 0;
        }
        {
            glAttachShader(program, vs_shader);
            auto status = glGetError();
            if (status != GL_NO_ERROR) {
                glDeleteProgram(program);
                glDeleteShader(vs_shader);
                glDeleteShader(fs_shader);
                return 0;
            }
        }
        {
            glAttachShader(program, fs_shader);
            auto status = glGetError();
            if (status != GL_NO_ERROR) {
                glDeleteProgram(program);
                glDeleteShader(vs_shader);
                glDeleteShader(fs_shader);
                return 0;
            }
        }


        if (link_program(program) > 0) {
            return program;
        } else {
            glDeleteProgram(program);
            glDeleteShader(vs_shader);
            glDeleteShader(fs_shader);
            return 0;
        }
    }


    GLuint create_shader_from_file(GLenum type, const std::string &path) {
        auto shader_source = utils::load_file(path);
        if (shader_source.empty()) {
            std::cerr << "Cannot read `" << path << "' file.\n";
            return 0;
        }
        auto shader = glCreateShader(type);
        if (shader == 0) {
            std::cerr << "Error creating shader" << "\n";
            return 0;
        }

        const char *source = shader_source.c_str();

        glShaderSource(shader, 1, &source, nullptr);

        glCompileShader(shader);
        GLint is_compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
        if (!is_compiled) {
            GLint max_log_length = 0u;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_log_length);
            std::string error_log;
            error_log.resize(max_log_length);
            glGetShaderInfoLog(shader, max_log_length, &max_log_length, &error_log[0]);
            glDeleteShader(shader);
            std::cerr << utils::shader_type(type) << " shader\n"
                      << " compilation error: " << error_log << std::endl;
            return 0;
        }
        return shader;
    }
}

