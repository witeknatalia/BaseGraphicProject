//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application/utils.h"

void SimpleShapeApplication::init() {

    set_camera(new Camera);

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    auto u_modifiers_index = glGetUniformBlockIndex(program,"Modifiers");
    if(u_modifiers_index == GL_INVALID_INDEX){
        std::cout<<"Cannot find Modifiers uniform block in program";
    }else{
        glUniformBlockBinding(program,u_modifiers_index,0);
    }

    auto u_transformations_index = glGetUniformBlockIndex(program,"Transformations");
    if(u_transformations_index == GL_INVALID_INDEX){
        std::cout<<"Cannot find Transformations uniform block in program"<<std::endl;
    }else{
        glUniformBlockBinding(program, u_transformations_index, 1);
    }

    int w, h;
    std::tie(w, h) = frame_buffer_size();

    float aspect_ = (float)w/h;
    float fov_ = glm::pi<float>()/4.0;
    float near_ = 0.1f;
    float far_ = 100.0f;
    camera()->perspective(fov_, aspect_, near_, far_);
    camera()->look_at(glm::vec3{1.0, 1.0, 1.2},
                    glm::vec3{0.0f, 0.0f, 0.0f},
                    glm::vec3{1.0, 1.0, 1.0}
    );

    std::vector<GLfloat> vertices = {
            0.0, 1.0, 0.0, 1.0, 0.0, 0.0,
            -1.0, -1.0, 1.0, 1.0, 0.0, 0.0,
            1.0, -1.0, 1.0, 1.0, 0.0, 0.0,

            0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
            1.0, -1.0, 1.0, 0.0, 1.0, 0.0,
            1.0, -1.0, -1.0, 0.0, 1.0, 0.0,

            0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
            1.0, -1.0, -1.0, 0.0, 0.0, 1.0,
            -1.0, -1.0, -1.0, 0.0, 0.0, 1.0,

            0.0, 1.0, 0.0, 1.0, 0.0, 1.0,
            -1.0, -1.0, -1.0, 1.0, 0.0, 1.0,
            -1.0, -1.0, 1.0, 1.0, 0.0, 1.0,

            -1.0, -1.0, 1.0, 1.0, 1.0, 0.0,
            1.0, -1.0, 1.0, 1.0, 1.0, 0.0,
            1.0, -1.0, 1.0, 1.0, 1.0, 0.0,

            1.0, -1.0, -1.0, 1.0, 1.0, 0.0,
            -1.0, -1.0, -1.0, 1.0, 1.0, 0.0,
    };

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    std::vector<GLushort> indices = {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 12
    };

    GLuint idx_buffer_handle;
    glGenBuffers(1,&idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort),indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


    GLuint ubo_handle(0);
    glGenBuffers(1, &ubo_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr,GL_STATIC_DRAW);

    float s = 0.5;
    glm::vec3 color(1.0, 1.0, 1.0);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &s);
    glBufferSubData(GL_UNIFORM_BUFFER, 4*sizeof(float), 3*sizeof(float), &color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &u_pvm_buffer_);

    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &camera()->projection()[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &camera()->view()[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(sizeof(GLfloat)*3));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    auto aspect = (float) w / h;
    camera()->set_aspect(aspect);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera()->zoom(yoffset / 30.0f);
}

void SimpleShapeApplication::frame() {
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &camera()->projection()[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &camera()->view()[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
}