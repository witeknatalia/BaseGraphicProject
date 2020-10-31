//
// Created by pbialas on 16.08.2020.
//
#include "Application/application.h"

#include <sstream>
#include <tuple>

#include "glad/glad.h"
#include "utils.h"


#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "3rdParty/src/stb/stb_image_write.h"

xe::Application::Application(int width, int height, std::string title, int major, int minor) : screenshot_n_(0) {

    if (glfwInit()) {

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);


        window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window_) {
            const char* error_desc;
            auto err_code = glfwGetError(&error_desc);
            std::cerr<<"Cannot create window: "<<error_desc<<"\n";
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(window_);
        glfwSwapInterval(1);
        glfwSetWindowUserPointer(window_, this);

        glfwSetFramebufferSizeCallback(window_, Application::glfw_framebuffer_size_callback);
        glfwSetScrollCallback(window_, Application::glfw_scroll_callback);
        glfwSetCursorPosCallback(window_, Application::glfw_cursor_position_callback);
        glfwSetMouseButtonCallback(window_, Application::glfw_mouse_button_callback);
        glfwSetKeyCallback(window_, Application::glfw_key_callback);
        glfwSetWindowRefreshCallback(window_, glfw_window_refresh_callback);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize OpenGL context" << std::endl;
            exit(-1);
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapInterval(1);

    }
}

void xe::Application::run(int verbose) {

    if (verbose > 0) {
        std::cout << utils::get_gl_description() << "\n";
    }
    init();


    auto macMoved = false;
    while (!glfwWindowShouldClose(window_)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* Render here */

        frame();
        /* Swap front and back buffers */
        glfwSwapBuffers(window_);

        /* Poll for and process events */
        glfwPollEvents();
#ifdef __APPLE__
        // A hack to fix bug in apple implementation.
        if (!macMoved) {
                    int x, y;
                    glfwGetWindowPos(window_, &x, &y);
                    glfwSetWindowPos(window_, ++x, y);
                    glfwSetWindowPos(window_, --x, y);
                    macMoved = true;
                }

#endif
    }

    cleanup();
    glfwTerminate();
}

void xe::Application::glfw_framebuffer_size_callback(GLFWwindow *window_ptr, int w, int h) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window_ptr));
    if (app_ptr) {
        app_ptr->framebuffer_resize_callback(w, h);
    }
}

void xe::Application::glfw_scroll_callback(GLFWwindow *window_ptr, double xoffset, double yoffset) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window_ptr));
    if (app_ptr) {
        app_ptr->scroll_callback(xoffset, yoffset);
    }
}

void xe::Application::glfw_cursor_position_callback(GLFWwindow *window, double x, double y) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->cursor_position_callback(x, y);
    }
}

void xe::Application::glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {

    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->mouse_button_callback(button, action, mods);
    }

}

void xe::Application::glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->key_callback(key, scancode, action, mods);
    }
}

void xe::Application::key_callback(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        std::cerr << "saving framebuffer" << "\n";
        save_frame_buffer();
    }
}


void xe::Application::save_frame_buffer() {
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    if (glGetError() == GL_INVALID_OPERATION) /* the display is single buffered */
        std::cerr << "Error setting FRAMEBUFFER\n";


    int w, h;
    std::tie(w, h) = frame_buffer_size();
    auto data = (GLubyte *) malloc(w * h * 3);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (glGetError() != GL_NO_ERROR)
        printf("Unknown error Reading Pixels\n");
    stbi_flip_vertically_on_write(1);
    std::stringstream ss;
    ss << "screenshot_" << std::setw(3) << std::setfill('0') << screenshot_n_ << ".png";
    stbi_write_png(ss.str().c_str(), w, h, 3, data, w * 3);
    ++screenshot_n_;
}

void xe::Application::glfw_window_refresh_callback(GLFWwindow *window) {

    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->window_refresh_callback();
    }
}

void xe::Application::window_refresh_callback() {}
