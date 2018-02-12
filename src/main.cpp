// Local Headers
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

void point_camera(float pitch, float yaw, glm::vec3* camera)
{
    camera->x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    camera->y = sin(glm::radians(pitch));
    camera->z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
}

int main(int argc, char** argv)
{
    sdl_wrapper::wrapper sdk(s_screen_width, s_screen_height);
    SDL_Window* window = sdk.window();

    glm::vec3 camera_pos(0.0, 0.0f, 3.0f);
    glm::vec3 camera_up( 0.0, 1.0f,  0.0f);
    glm::vec3 camera_front(0.0, 0.0f, -1.0f);

    float camera_speed = 0.02;
    float camera_rot_speed = 0.05;
    // TODO: Convert to quaternian based system
    float camera_pitch = 0.0;
    float camera_yaw = 270.0;

    glm::mat4 view = glm::lookAt(camera_pos, camera_front, camera_up);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)s_screen_width / (float)s_screen_height,
                                  0.1f, 100.0f);

    cube voxel_cube;
    voxel_cube.set_projection(projection);

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
                        case SDLK_w:
                            camera_pos += camera_speed * delta * camera_front;
                            break;

                        case SDLK_s:
                            camera_pos -= camera_speed * delta * camera_front;
                            break;

                        case SDLK_a:
                            camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed * delta;
                            break;

                        case SDLK_d:
                            camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed * delta;
                            break;

                        case SDLK_UP:
                            camera_pitch += camera_rot_speed * delta;
                            break;

                        case SDLK_DOWN:
                            camera_pitch -= camera_rot_speed * delta;
                            break;

                        case SDLK_LEFT:
                            camera_yaw -= camera_rot_speed * delta;
                            break;

                        case SDLK_RIGHT:
                            camera_yaw += camera_rot_speed * delta;
                            break;

                        case SDLK_j:
                            printf("%f, %f\n", camera_pitch, camera_yaw);
                    }
                    point_camera(camera_pitch, camera_yaw, &camera_front);
            }
        }

        gl_wrapper::clear_screen();

        view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

        for (int i = 0; i < 10; i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, positions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            voxel_cube.draw(model, view);
        }

        SDL_GL_SwapWindow(window);
    }

    return 0;
}
