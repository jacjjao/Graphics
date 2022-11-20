#version 460 core

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec4 in_color; 

uniform mat3 model;
uniform mat3 view;

out vec4 frag_color;

void main()
{
	gl_Position = vec4(model * vec3(in_pos, 1.0), 1.0);
	frag_color = in_color;
}
