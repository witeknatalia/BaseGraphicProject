//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "camera.h"
#include "camera_controler.h"
#include "pyramid.h"

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    void set_camera(Camera *camera) { this->camera_ = camera; }

    void set_controler(CameraControler *controler) { this->controler_ = controler; }


    Camera *camera() { return this->camera_; }
    ~SimpleShapeApplication() {
        if (this->camera_) {
            delete this->camera_;
        }
    }

    void scroll_callback(double xoffset, double yoffset) override;
    Pyramid *pyramid;

private:
    Camera *camera_;
    CameraControler *controler_;

    GLuint vao_;
    GLuint u_pvm_buffer_;
};
