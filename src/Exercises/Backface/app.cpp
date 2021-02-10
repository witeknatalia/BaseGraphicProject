//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

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

    std::vector<GLushort> indices = {
            12, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
    };

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    GLuint idx_buffer_handle;
    glGenBuffers(1,&idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


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

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));

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
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
}
