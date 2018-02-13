// Module Header
#include "camera.hpp"

// External Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

camera::camera() :
    m_pitch(0),
    m_yaw(270),
    m_trans_speed(0.03),
    m_rot_speed(0.05),
    m_pos(0.0f, 0.0f, 3.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_front(0.0f, 0.0f, -1.0f),
    m_view(1.0f)
{
    m_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);

    refresh_view();
}

const glm::mat4& camera::view()
{
    return m_view;
}

void camera::move_forward(float delta_t)
{
    m_pos += m_trans_speed * delta_t * m_front;
    refresh_view();
}

void camera::move_back(float delta_t)
{
    m_pos -= m_trans_speed * delta_t * m_front;
    refresh_view();
}

void camera::move_left(float delta_t)
{
    glm::vec3 dir = glm::cross(m_front, m_up);
    m_pos -= glm::normalize(dir) * m_rot_speed * delta_t;
    refresh_view();
}

void camera::move_right(float delta_t)
{
    glm::vec3 dir = glm::cross(m_front, m_up);
    m_pos += glm::normalize(dir) * m_rot_speed * delta_t;
    refresh_view();
}

void camera::pitch_up(float delta_t)
{
    m_pitch += m_rot_speed * delta_t;
    refresh_view();
}

void camera::pitch_down(float delta_t)
{
    m_pitch -= m_rot_speed * delta_t;
    refresh_view();
}

void camera::yaw_left(float delta_t)
{
    m_yaw -= m_rot_speed * delta_t;
    refresh_view();
}

void camera::yaw_right(float delta_t)
{
    m_yaw += m_rot_speed * delta_t;
    refresh_view();
}

void camera::refresh_view()
{
    m_front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

    m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
}
