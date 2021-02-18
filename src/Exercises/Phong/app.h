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
//#include "pyramid.h"
#include "quad.h"


struct Light {
    glm::vec4 position;
    glm::vec4 color;
    glm::vec4 a;
    glm::vec4 ambient;
};


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    void set_camera(Camera *camera) { camera_ = camera; }
    void set_controler(CameraControler *controler) { controler_ = controler; }
    Camera * camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void scroll_callback(double xoffset, double yoffset) override;
    Quad *quad_;

private:
    Camera * camera_;
    CameraControler * controler_;

    GLuint vao_;
    GLuint u_pvm_buffer_;
    GLuint u_light_buffer_;

    Light light_;

};
