//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>


#include "Application/utils.h"

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    std::vector<GLfloat> vertices = {
            -0.2, -0.2, 0.0, 0.0, 1.0, 0.0,
            0.2, -0.2, 0.0, 0.0, 1.0, 0.0,
            -0.2, 0.2, 0.0, 0.0, 1.0, 0.0,
            0.2, 0.2, 0.0, 1.0, 0.0, 0.0,
            0.0, 0.7, 0.0, 1.0, 0.0, 0.0,
            -0.2, 0.2, 0.0, 1.0, 0.0, 0.0,
            0.2, 0.2, 0.0, 0.0, 1.0, 0.0,
    };

    std::vector<GLushort> indices = {
            0, 1, 2, 3, 4, 5, 6, 2, 1
    };

    GLuint idx_buffer_handle;
    glGenBuffers(1,&idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
    if (u_modifiers_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
    } else {
        glUniformBlockBinding(program, u_modifiers_index, 0);
    }

    GLuint ubo_handle(0u);
    glGenBuffers(1,&ubo_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    float s = 0.5;

    glBufferSubData(GL_UNIFORM_BUFFER, 0, 1 * sizeof(float), &s);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle);


    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
}
