#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec4 in_color; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 ortho;

out vec4 frag_color;

void main()
{
	gl_Position = vec4(in_pos, 1.0);
	frag_color = in_color;
}
