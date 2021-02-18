
#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Application/utils.h"
#include "quad.h"
#include "quad.cpp"


void SimpleShapeApplication::init() {

    set_camera(new Camera);
    set_controler(new CameraControler(this->camera()));

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    auto u_pvm_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_pvm_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find PVM uniform block in program" << std::endl;
    } else {
        glUniformBlockBinding(program, u_pvm_index, 1);
    }

    auto  u_diffuse_map_location = glGetUniformLocation(program, "diffuse_map");
    if(u_diffuse_map_location == -1) {
        std::cerr << "Cannot find uniform diffuse_map" << std::endl;
    } else {
        glUniform1ui(u_diffuse_map_location, 0);
    }


    int w, h;
    std::tie(w, h) = frame_buffer_size();

    float aspect_ = (float)w/h;
    float fov_ = glm::pi<float>()/3.0;
    float near_ = 0.5f;
    float far_ = 100.0f;
    camera()->perspective(fov_, aspect_, near_, far_);

    camera()->look_at(glm::vec3{1.0, 1.0, 1.2},
                      glm::vec3{0.0f, 0.0f, 0.0f},
                      glm::vec3{1.0, 1.0, 1.0}
    );

    quad_ = new Quad;

    GLuint ubo_handle(0u);
    glGenBuffers(1, &ubo_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);


    glGenBuffers(1, &u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 3 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &camera()->projection()[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &camera()->view()[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    auto u_light_index = glGetUniformBlockIndex(program,"Light");
    if(u_light_index == -1){
        std::cerr<<"Cannot find uniform Light\n";
    }else{
        glUniformBlockBinding(program,u_light_index,2);
    }

    glGenBuffers(1, &u_light_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);

    light_.a = glm::vec4 (1.0f, 1.0f, 0.0f, 0.0f);
    light_.color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    light_.position = camera()->view() * glm::vec4(0.0f,1.0f,0.0f,1.0f);
    light_.ambient = glm::vec4(0.2f,0.2f,0.2f,0.0f);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(GLfloat), &light_.position[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(GLfloat), 4 * sizeof(GLfloat), &light_.color[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), 4 * sizeof(GLfloat), &light_.light1[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 12 * sizeof(GLfloat), 4 * sizeof(GLfloat), &light_.ambient[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer_);



    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);
    glFrontFace(GL_CW);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
    }


void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    auto aspect = (float) w / h;
    camera()->set_aspect(aspect);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera()->zoom(yoffset / 30.0f);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }

}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}

void SimpleShapeApplication::frame() {
    auto VM = camera()->view();
    auto P = camera()->projection();
    auto R = glm::mat3(VM);
    auto N = glm::transpose(glm::inverse(R));


    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]);;
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec4), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec4), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec4), &N[2]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    auto l = VM * glm::vec4(0.0f,0.0f,0.1f,1.0f);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &l[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(GLfloat), 4 * sizeof(GLfloat), &light_.color[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), 4 * sizeof(GLfloat), &light_.a[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 12 * sizeof(GLfloat), 4 * sizeof(GLfloat),&light_.ambient[0]);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    quad_->draw();
}

