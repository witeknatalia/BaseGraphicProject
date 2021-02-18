
#include "pyramid.h"
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/src/stb/stb_image.h"

Pyramid::Pyramid() {

    std::vector<GLfloat> vertices = {
            0.0, 1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 1.0, 0.0,
            0.0, 1.0, 0.0, 1.0, 1.0,
            0.0, 1.0, 0.0, 0.0, 1.0,

            -1.0, -1.0, 1.0, 0.191, 0.50,
            1.0, -1.0, 1.0, 0.50, 0.191,
            1.0, -1.0, -1.0, 0.809, 0.50,
            -1.0, -1.0, -1.0, 0.50, 0.809,


    };


    glGenBuffers(2, buffers_);
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::vector<GLushort> indices = {
            0,4,5,1,5,6,2,6,7,3,7,4,4,5,7,5,6,7
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(sizeof(GLfloat)*3));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[1]);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int width, height, n_channels;
    auto texture_filename = std::string(PROJECT_DIR) + "/Textures/multicolor.png";
    uint8_t  *data = stbi_load(texture_filename.c_str(), &width, &height, &n_channels, 0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

}

Pyramid::~Pyramid() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(2, buffers_);
}

void Pyramid::draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
