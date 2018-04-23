#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GL/glew.h>

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

const float CAM_DEFAULT_YAW = -90.0f;
const float CAM_DEFAULT_PITCH = 0.0f;
const float CAM_DEFAULT_MOVEMENT_SPEED = 300.0f;
const float CAM_DEFAULT_SENSITIVITY = 0.15f;
const float CAM_DEFAULT_ZOOM = 90.0f;

class Camera
{
private:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    float yaw_;
    float pitch_;

    float movement_speed_;
    float mouse_sensitivity_;
    float zoom_;

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = CAM_DEFAULT_YAW,
           float pitch = CAM_DEFAULT_PITCH);

    Camera(float pos_x, float pos_y, float pos_z,
           float up_x, float up_y, float up_z,
           float yaw, float pitch);

    ~Camera();

    glm::mat4 get_view_matrix() const;

    void process_keyboard(CameraMovement direction, float delta_time);
    void process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch = true);
    void process_mouse_scroll(float y_offset);

    inline float get_zoom() const { return zoom_; }

private:
    void update_camera_vectors();
};

