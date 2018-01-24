#ifndef GL_WRAPPER_HPP
#define GL_WRAPPER_HPP

// Local Headers
#include "glad/glad.h"

class gl_wrapper_shader {
 public:
    gl_wrapper_shader(GLenum shaderType);
    ~gl_wrapper_shader();

 private:
    GLuint m_handle;
};

#endif // GL_WRAPPER_HPP
