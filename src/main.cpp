// Local Headers
#include "gl_wrapper.hpp"
#include "sdl_wrapper.hpp"

// External Headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

// C Standard Headers
#include <cmath>
#include <cstdint>
#include <cstdio>

// CPP Standard Headers
#include <string>

using namespace std;

//Screen dimension constants
static int s_screen_width = 3200;
static int s_screen_height = 1800;

static float s_vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

static void enable_vertex_attrib()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

static void enable_texture_attrib()
{
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

static float time()
{
    uint32_t ticks = SDL_GetTicks();
    return float(ticks) / 1000;
}

static float s_trans = 0.2;
static float s_roll = 0.0;
static float s_pitch = 0.0;

static void increase(float *value)
{
    if (*value <= 0.9) {
        *value += 0.1;
    }
}

static void decrease(float *value)
{
    if (*value >= -0.9) {
        *value -= 0.1;
    }
}

int main(int argc, char** argv)
{
    sdl_wrapper::wrapper sdk(s_screen_width, s_screen_height);
    SDL_Window* window = sdk.window();

    gl_wrapper::vao box_vao;
    gl_wrapper::vbo box_vbo;

    box_vao.bind();
    box_vbo.load(s_vertices, sizeof(s_vertices));

    enable_vertex_attrib();
    enable_texture_attrib();

    gl_wrapper::shader_program program("vertex.glsl", "fragment.glsl");

    gl_wrapper::texture texture0("container.jpg", false);
    gl_wrapper::texture texture1("awesomeface.png", true);

    program.use();
    program.set_uniformi("texture0", 0);
    program.set_uniformi("texture1", 1);
    program.set_uniformf("trans", (float)0.2);

    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)s_screen_width / (float)s_screen_height, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        glViewport(0, 0, e.window.data1, e.window.data2);
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_s:
                            increase(&s_pitch);
                            break;
                        case SDLK_w:
                            decrease(&s_pitch);
                            break;
                        case SDLK_d:
                            increase(&s_roll);
                            break;
                        case SDLK_a:
                            decrease(&s_roll);
                            break;
                        case SDLK_j:
                            s_roll = 0;
                            s_pitch = 0;
                            break;
                    }
            }
        }

        program.use();
        box_vao.bind();

        model = glm::rotate(model, (float)(s_pitch * time() * 0.001), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(s_roll * time() * 0.001), glm::vec3(0.0f, 1.0f, 0.0f));
        program.set_uniform4fv("model", glm::value_ptr(model));
        program.set_uniform4fv("view", glm::value_ptr(view));
        program.set_uniform4fv("projection", glm::value_ptr(projection));

        gl_wrapper::clear_screen();

        glActiveTexture(GL_TEXTURE0);
        texture0.bind();

        glActiveTexture(GL_TEXTURE1);
        texture1.bind();

        glDrawArrays(GL_TRIANGLES, 0, 36);
        SDL_GL_SwapWindow(window);
    }

    return 0;
}
