#pragma once

#include "glad/glad.h"

class Pyramid {
public:
    Pyramid();
    ~Pyramid();
    void draw();
private:
    GLuint vao_;
    GLuint buffers_[2];
    GLuint diffuse_texture_;
};