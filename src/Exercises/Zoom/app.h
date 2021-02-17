//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"
#include "Camera.h"

#include "glad/glad.h"
#include <glm/glm.hpp>

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }
    void scroll_callback(double xoffset, double yoffset) override;
private:
    GLuint vao_;
    Camera * camera_;
    glm::mat4 P_;
    glm::mat4 V_;
    GLuint u_pvm_buffer_;
};