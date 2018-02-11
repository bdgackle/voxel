#version 330 core

out vec4 frag_color;

in vec3 out_color;
in vec2 out_tex_coord;

uniform sampler2D out_texture;

void main()
{
    frag_color = texture(out_texture, out_tex_coord) * vec4(out_color, 1.0);
}
