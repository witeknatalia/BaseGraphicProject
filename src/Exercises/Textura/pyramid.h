#pragma once

#include "glad/glad.h"

class Pyramid {
public:
    Pyramid();

    Pyramid(const Pyramid& rhs) = delete;
    Pyramid &operator = (const Pyramid& rhs) = delete;

    void operator = (Pyramid&& rhs) = delete;
    Pyramid(Pyramid&& rhs) = delete;

    void draw();

    ~Pyramid();

private:
    GLuint vao_;
    GLuint buffer_[2];
};
