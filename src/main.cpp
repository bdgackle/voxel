// Local Headers
#include "gl_wrapper.hpp"
#include "sdl_wrapper.hpp"

// External Headers
#include <glad/glad.h>
#include <SDL2/SDL.h>

// C Standard Headers
#include <cmath>
#include <cstdint>
#include <cstdio>

// CPP Standard Headers
#include <string>

using namespace std;

//Screen dimension constants
static int s_screen_width = 640;
static int s_screen_height = 480;

static float s_vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
};

static unsigned int s_indices[] = {
    0, 1, 3,
    1, 2, 3
};

static unsigned int _create_vao(void)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_indices), s_indices, GL_STATIC_DRAW);

    // Setup vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return vao;
}

int main(int argc, char** argv)
{
    sdl_wrapper::wrapper sdk;
    SDL_Window* window = sdk.window();

    // Initial viewport call (should be tied to SDL window resize callback)
    glViewport(0, 0, s_screen_width, s_screen_height);

    // Set background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    gl_wrapper::shader_program program("vertex.glsl", "fragment.glsl");
    unsigned int vao = _create_vao();

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    glViewport(0, 0, e.window.data1, e.window.data2);
                }
            }
        }

        // Time varying sinusoid @ 1Hz, value 0 to 1;
        uint32_t ticks = SDL_GetTicks();
        float seconds = (float)ticks / 1000;
        float green_value = (sin(seconds) / 2.0f) + 0.5f;

        int color_location = glGetUniformLocation(program.handle(), "color");
        glUseProgram(program.handle());
        glUniform4f(color_location, 0.0f, green_value, 0.0f, 1.0f);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        SDL_GL_SwapWindow(window);
    }

    return 0;
}
