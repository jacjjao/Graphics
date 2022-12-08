#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec4 in_color; 
layout (location = 2) in vec2 in_tex_coord;

uniform mat4 model;
uniform mat4 view;

out vec4 frag_color;
out vec2 tex_coord;

void main()
{
	gl_Position = view * model * vec4(in_pos, 1.0);
	frag_color = in_color;
	tex_coord = in_tex_coord;
}
