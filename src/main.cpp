// Local Headers
#include "camera.hpp"
#include "cube.hpp"
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

static glm::vec3 positions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

int main(int argc, char** argv)
{
    sdl_wrapper::wrapper sdk(s_screen_width, s_screen_height);
    SDL_Window* window = sdk.window();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f),
                                      (float)s_screen_width / (float)s_screen_height,
                                      0.1f, 100.0f);

    cube voxel_cube;
    camera cam;

    voxel_cube.set_projection(proj);

    bool quit = false;
    uint32_t last_frame = SDL_GetTicks();
    while (!quit) {
        uint32_t current_frame = SDL_GetTicks();
        float delta = current_frame - last_frame;
        last_frame = current_frame;

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
                        case SDLK_w:        cam.move_forward(delta); break;
                        case SDLK_s:        cam.move_back(delta);    break;
                        case SDLK_a:        cam.move_left(delta);    break;
                        case SDLK_d:        cam.move_right(delta);   break;
                        case SDLK_UP:       cam.pitch_up(delta);     break;
                        case SDLK_DOWN:     cam.pitch_down(delta);   break;
                        case SDLK_LEFT:     cam.yaw_left(delta);     break;
                        case SDLK_RIGHT:    cam.yaw_right(delta);    break;
                        default: /* No action */                     break;
                    }
            }
        }

        gl_wrapper::clear_screen();

        for (int i = 0; i < 10; i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, positions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            voxel_cube.draw(model, cam.view());
        }

        SDL_GL_SwapWindow(window);
    }

    return 0;
}
