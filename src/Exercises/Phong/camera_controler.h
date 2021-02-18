#include "camera.h"

class CameraControler {
public:
    CameraControler():camera_(nullptr) {
        LMB_pressed_ = false;
        scale_=0.04f;
    }

    CameraControler(Camera* camera):camera_(camera) {
        LMB_pressed_ = false;
        scale_=0.04f;
    }

    void set_camera(Camera *camera) { camera_ = camera; }

    void rotate_camera(float dx, float dy) {
        camera_->rotate_around_center(-scale_ * dy, camera_->x());
        camera_->rotate_around_center(-scale_ * dx, camera_->y());
    }

    void mouse_moved(float x, float y) {
        if (LMB_pressed_) {
            auto dx = x - x_;
            auto dy = y - y_;
            x_ = x;
            y_ = y;

            rotate_camera(dx, dy);
        }
    };

    void LMB_pressed(float x, float y) {
        LMB_pressed_ = true;
        x_ = x;
        y_ = y;
    };

    void LMB_released(float x, float y) {
        LMB_pressed_ = false;
        auto dx = x - x_;
        auto dy = y - y_;

        rotate_camera(dx, dy);
    };

private:
    Camera *camera_;

    float scale_;
    bool LMB_pressed_;
    float x_,y_;
};