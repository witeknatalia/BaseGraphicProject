

#pragma once

#include "glad/glad.h"

class Quad {
public:
    Quad();
    ~Quad();
    void draw();
    Quad(const Quad& rhs)=delete;
    Quad &operator =(const Quad& rhs) = delete;
    void operator=(Quad&& rhs)=delete;
    Quad(Quad &&rhs)=delete;

private:
    GLuint vao_;
    GLuint buffers_[2];
    GLuint diffuse_texture_;
};
