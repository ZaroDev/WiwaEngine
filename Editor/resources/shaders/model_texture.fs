#version 330 core

uniform Sampler2D gSampler;


in vec2 o_texCoord;
out vec4 out_color;


void main() {
	out_color = texture2D(gSampler, o_texCoord);
}