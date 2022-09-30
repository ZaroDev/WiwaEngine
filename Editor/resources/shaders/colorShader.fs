#version 330 core

uniform vec4 u_Color;

in vec4 f_Color;

out vec4 out_color;

void main() {
	out_color = f_Color;
}