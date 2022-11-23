#version 460 core

in vec4 frag_color;
in vec2 tex_coord;

out vec4 color;

uniform bool apply_texture;

uniform sampler2D texture1;

void main()
{	
	if (apply_texture)
		color = texture(texture1, tex_coord);
	else 
		color = frag_color;
}