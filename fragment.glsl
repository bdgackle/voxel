#version 330 core

out vec4 frag_color;

in vec2 vert_tex_coord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float trans;

void main()
{
    frag_color = mix(texture(texture0, vert_tex_coord),
                     texture(texture1, vert_tex_coord),
                     trans);
}
