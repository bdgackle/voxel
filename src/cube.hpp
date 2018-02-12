#ifndef CUBE_HPP
#define CUBE_HPP

// Local Headers
#include "gl_wrapper.hpp"

// External Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// C++ Standard Headers
#include <string>

enum class cube_texture {
    crate
};

class cube {
 public:
    cube();

    void set_projection(const glm::mat4& projection_mat);

    void draw(const glm::mat4& model, const glm::mat4& view);

 private:
    gl_wrapper::vao m_vao;
    gl_wrapper::vbo m_vbo;
    gl_wrapper::shader_program m_shader_program;
    gl_wrapper::texture m_texture;
    glm::mat4 m_projection;

    static const int m_vertex_count = 36;
    static const float m_vertex_data[m_vertex_count * 5];
    static const std::string m_vertex_shader_filename;
    static const std::string m_fragment_shader_filename;
    static const std::string m_texture_filename;
};

#endif // CUBE_HPP
