
#include <vector>
#include "quad.h"
#include "3rdParty/src/stb/stb_image.h"

Quad ::Quad() {

    int width, height, n_channels;

    auto texture_filename = std::string(PROJECT_DIR) + "/Textures/silver.png";

    uint8_t *data = stbi_load(texture_filename.c_str(),&width,&height,&n_channels,0);

    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::vector<GLfloat> vertices = {
            -1.0, -1.0, 0.0, 0.0, 1.0, 0.0,
            1.0, -1.0, 0.0, 0.0, 1.0, 0.0,
            -1.0, 1.0, 0.0, 0.0, 1.0, 0.0,
            1.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    };


    glGenBuffers(2, buffers_);
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::vector<GLushort> indices={0, 1, 2, 2, 1, 3};

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat),reinterpret_cast<GLvoid *>( 3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<GLvoid *>( 5 * sizeof(GL_FLOAT)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[1]);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
Quad::~Quad() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(2, buffers_);
}
void Quad::draw() {
    glBindVertexArray(vao_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
}

