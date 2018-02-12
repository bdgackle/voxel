#ifndef GL_WRAPPER_HPP
#define GL_WRAPPER_HPP

// Local Headers
#include "glad/glad.h"

// C++ Standard Headers
#include <string>

namespace gl_wrapper {

/**
 *  RAII wrapper class for an OpenGL VAO
 */
class vao {
 public:
    vao();
    ~vao();

    void bind();

 private:
    GLuint m_handle;
};

/**
 *  RAII wrapper class for an OpenGL VBO
 */
class vbo {
 public:
    vbo();
    ~vbo();

    void bind();
    void load(const GLvoid *data, GLsizeiptr size);

 private:
    GLuint m_handle;
};

/**
 *  RAII wrapper class for an OpenGL EBO
 */
class ebo {
 public:
    ebo();
    ~ebo();

    void bind();
    void load(const GLvoid *data, GLsizeiptr size);

 private:
    GLuint m_handle;
};

/**
 *  RAII wrapper class for an OpenGL shader object
 */
class shader {
    friend class shader_program;

 private:
    shader(GLenum shaderType);
    ~shader();

    void compile(const std::string& filename);
    bool compile_success();
    void print_compile_msg();

    GLuint m_handle;
};

/**
 *  RAII wrapper class for an OpenGL program object
 */
class program {
    friend class shader_program;

 private:
    program();
    ~program();

    GLuint handle() const;

    void link(GLuint vertex_shader, GLuint fragment_shader);

    GLuint m_handle;
};

/**
 *  Wrapper class for an OpenGL program and associated shaders
 */
class shader_program {
 public:
    shader_program(const std::string& vertex_filename,
                   const std::string& fragment_filename);
    void use();
    GLuint handle();
    void set_uniformi(const std::string& name, int value);
    void set_uniformf(const std::string& name, float value);
    void set_uniform4fv(const std::string& name, const float *value);

 private:
    program m_program;
    shader m_vertex_shader;
    shader m_fragment_shader;
};

/**
 *  RAII wrapper class for an STBI image object
 */
class image {
    friend class texture;

 private:
    image(const std::string& image_filename);
    ~image();

    unsigned char *data();
    int width() const;
    int height() const;
    int channels() const;

    unsigned char *m_image_data;
    int m_width;
    int m_height;
    int m_channels;
};

/**
 *  Wrapper class for an OpenGL texture
 */
class texture {
 public:
    texture(const std::string& image_filename, bool has_alpha);
    ~texture() = default;

    void bind();

 private:
    GLuint m_handle;
    image m_image;
};

/**
 *  Convenience function: converts a text file to a std::string
 */
std::string read_shader_source(const std::string& filename);

/**
 *  Convenience function for OpenGL calls to clear screen
 */
void clear_screen();

} // namespace gl_wrapper

#endif // GL_WRAPPER_HPP
