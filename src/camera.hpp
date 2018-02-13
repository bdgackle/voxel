#ifndef CAMERA_HPP
#define CAMERA_HPP

// External Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class camera {
 public:
    camera();

    const glm::mat4& view();

    void move_forward(float delta_t);
    void move_back(float delta_t);
    void move_left(float delta_t);
    void move_right(float delta_t);

    void pitch_up(float delta_t);
    void pitch_down(float delta_t);
    void yaw_left(float delta_t);
    void yaw_right(float delta_t);

    void refresh_view();

 private:
    float m_pitch;
    float m_yaw;

    float m_trans_speed;
    float m_rot_speed;

    glm::vec3 m_pos;
    glm::vec3 m_up;
    glm::vec3 m_front;

    glm::mat4 m_view;
};

#endif // CAMERA_HPP
