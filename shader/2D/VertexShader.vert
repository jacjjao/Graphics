#version 460 core

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec4 in_color; 

uniform mat4 model;
uniform mat4 view;

out vec4 frag_color;

void main()
{
	gl_Position = model * vec4(in_pos, 0.0, 1.0);
	frag_color = in_color;
}
