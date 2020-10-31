//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <string>

#include "glad/glad.h"

namespace xe {
    namespace utils {

        std::string load_file(const std::string &path);

        std::string get_gl_description(void);

        std::string shader_type(GLenum type);

    }

    GLuint create_shader_from_file(GLenum type, const std::string &path);

    GLuint link_program(GLuint program);

    GLuint create_program(const std::string &vs_path, const std::string &fs_path);



}