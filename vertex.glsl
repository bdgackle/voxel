#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 tex_coord;

out vec3 out_color;
out vec2 out_tex_coord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    out_color = in_color;
    out_tex_coord = tex_coord;
}
