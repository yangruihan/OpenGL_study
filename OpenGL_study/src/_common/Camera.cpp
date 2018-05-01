#include "Camera.h"

Camera::Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch)
    : position_(position), front_(0.0f, 0.0f, -1.0f), world_up_(up),
      yaw_(yaw), pitch_(pitch), movement_speed_(CAM_DEFAULT_MOVEMENT_SPEED),
      mouse_sensitivity_(CAM_DEFAULT_SENSITIVITY), zoom_(CAM_DEFAULT_ZOOM)
{
    update_camera_vectors();
}

Camera::Camera(const float pos_x, const float pos_y, const float pos_z,
               const float up_x,  const float up_y,  const float up_z,
               const float yaw,   const float pitch)
    : position_(glm::vec3(pos_x, pos_y, pos_z)),
      world_up_(glm::vec3(up_x, up_y, up_z)),
      yaw_(yaw), pitch_(pitch), movement_speed_(CAM_DEFAULT_MOVEMENT_SPEED),
      mouse_sensitivity_(CAM_DEFAULT_SENSITIVITY), zoom_(CAM_DEFAULT_ZOOM)
{
    update_camera_vectors();
}

Camera::~Camera() = default;

glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::process_keyboard(const CameraMovement direction, const float delta_time)
{
    const auto velocity = movement_speed_ * delta_time;

    if (direction == FORWARD)
        position_ += front_ * velocity;

    if (direction == BACKWARD)
        position_ -= front_ * velocity;

    if (direction == LEFT)
        position_ -= right_ * velocity;

    if (direction == RIGHT)
        position_ += right_ * velocity;

    if (direction == UP)
        position_ += up_ * velocity;

    if (direction == DOWN)
        position_ -= up_ * velocity;
}

void Camera::process_mouse_movement(float x_offset, float y_offset, const GLboolean constrain_pitch)
{
    x_offset *= mouse_sensitivity_;
    y_offset *= mouse_sensitivity_;

    yaw_ += x_offset;
    pitch_ += y_offset;


    if (constrain_pitch)
    {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;

        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    update_camera_vectors();
}

void Camera::process_mouse_scroll(const float y_offset)
{
    if (zoom_ >= 1.0f && zoom_ <= 180.0f)
        zoom_ -= y_offset;

    if (zoom_ <= 1.0f)
        zoom_ = 1.0f;

    if (zoom_ >= 180.0f)
        zoom_ = 180.0f;
}

void Camera::update_camera_vectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
}
