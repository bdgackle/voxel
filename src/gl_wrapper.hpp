#ifndef GL_WRAPPER_HPP
#define GL_WRAPPER_HPP

// Local Headers
#include "glad/glad.h"

// C++ Standard Headers
#include <string>

namespace gl_wrapper {

class shader {
    friend class shader_program;

 private:
    shader(GLenum shaderType);
    ~shader();

    void compile(std::string filename);
    bool compile_success();
    void print_compile_msg();

    GLuint m_handle;
};

class program {
    friend class shader_program;

 private:
    program();
    ~program();

    void link(GLuint vertex_shader, GLuint fragment_shader);

    GLuint m_handle;
};

class shader_program {
 public:
    shader_program(std::string vertex_filename, std::string fragment_filename);

    GLuint handle();


 private:
    program m_program;
    shader m_vertex_shader;
    shader m_fragment_shader;
};

std::string read_shader_source(std::string filename);

} // namespace gl_wrapper

#endif // GL_WRAPPER_HPP
