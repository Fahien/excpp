#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( binding = 1 ) uniform sampler2D color_texture;

layout( location = 0 ) in vec4 in_color;
layout( location = 1 ) in vec2 in_texcoord;

layout( location = 0 ) out vec4 out_color;

void main()
{
	out_color = texture( color_texture, in_texcoord );
}
