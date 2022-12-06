#version 460 core

in vec4 frag_color;
in vec2 tex_coord;

out vec4 color;

uniform float color_alpha;
uniform sampler2D texture1;

void main()
{	
	color = mix(texture(texture1, tex_coord), frag_color, color_alpha);
}