// External Headers
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// C Standard Headers
#include <stdio.h>

// CPP Standard Headers
#include <string>

using namespace std;

//Screen dimension constants
static int s_screen_width = 640;
static int s_screen_height = 480;

SDL_Window* s_window;
SDL_GLContext s_context;

static bool _sdl_init(void)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

static void _sdl_cleanup()
{
    if (s_window != nullptr) {
        SDL_DestroyWindow(s_window);
        s_window = nullptr;
    }

    if (s_context != nullptr) {
        SDL_GL_DeleteContext(s_context);
        s_context = nullptr;
    }

    //Quit SDL subsystems
    SDL_Quit();
}

static bool _sdl_create_main_window(void)
{
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) != 0) {
        printf("Error setting SDL_GL_CONTEXT_MAJOR_VERSION\n");
        return false;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) != 0) {
        printf("Error setting SDL_GL_CONTEXT_MINOR_VERSION\n");
        return false;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
        printf("Error setting SDL_GL_CONTEXT_PROFILE_MASK\n");
        return false;
    }

    // Create main window
    s_window = SDL_CreateWindow("OpenGL Tutorial",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                s_screen_width,
                                s_screen_height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (s_window == nullptr) {
        printf("Error creating OpenGL window: %s.\n", SDL_GetError());
        return false;
    }

    return true;
}

static bool _sdl_create_opengl_context(void)
{
    s_context = SDL_GL_CreateContext(s_window);
    if (s_context == nullptr) {
        printf("Error creating OpenGL context: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Setting swap intervale not supported.\n");
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return false;
    }

    return true;
}

static bool _init(void)
{
    if (!_sdl_init()) {
        printf("SDL init failed.\n");
        return false;
    }

    if (!_sdl_create_main_window()) {
        printf("SDL window creation failed.\n");
        return false;
    }

    if (!_sdl_create_opengl_context()) {
        printf("OpenGL context creation failed.\n");
        return false;
    }

    return true;
}

static void _gl_set_viewport(void)
{
    glViewport(0, 0, s_screen_width, s_screen_height);
}

static void _update_window_size(int32_t width, int32_t height)
{
    s_screen_width = width;
    s_screen_height = height;
    _gl_set_viewport();
}

static const char* const vertex_shader_source = "\
#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
\
void main()\n\
{\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
}\n\
";

static const char* const fragment_shader_source = "\
#version 330 core\n\
out vec4 FragColor;\n\
\n\
void main()\n\
{\n\
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
}\n\
";

static void _print_shader_compilation_result(unsigned int shader_id)
{
    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    printf("Compile shader %u: %s\n", shader_id, success == 0 ? "Fail" : "Okay");

    if (success == 0) {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        printf("SHADER COMPILE FAILED:\n%s\n", info_log);
    }
}

static unsigned int _create_vertex_shader(void)
{
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    _print_shader_compilation_result(vertex_shader);

    return vertex_shader;
}

static unsigned int _create_fragment_shader(void)
{
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    _print_shader_compilation_result(fragment_shader);

    return fragment_shader;
}

static unsigned int _create_shader_program(void)
{
    unsigned int vertex_shader = _create_vertex_shader();
    unsigned int fragment_shader = _create_fragment_shader();
    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;

}

static unsigned int _create_vao(void)
{
    // Define vertices of a single flat triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Create VAO
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a VBO, bind it, and copy data to it
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GL_DYNAMIC_DRAW -- performance?

    // Setup vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return vao;
}

int main(int argc, char** argv)
{
    if (!_init()) {
        printf("Exiting due to failed initialization.\n");
        return 1;
    }

    // Initial viewport call (should be tied to SDL window resize callback)
    glViewport(0, 0, s_screen_width, s_screen_height);

    // Set background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned int program = _create_shader_program();
    unsigned int vao = _create_vao();

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    _update_window_size(e.window.data1, e.window.data2);
                }
            }
        }

        SDL_GL_SwapWindow(s_window);
    }

    //Free resources and close SDL
    _sdl_cleanup();

    return 0;
}
