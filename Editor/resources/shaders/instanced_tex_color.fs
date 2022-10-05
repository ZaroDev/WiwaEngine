#version 330 core

uniform sampler2D u_Textures[2];

in vec4 f_Color;
in vec2 f_TexCoord;
in float f_TexID;

out vec4 out_color;

void main() {
	vec2 texCoordFlip = f_TexCoord;
	int index = int(f_TexID);

	out_color = texture2D(u_Textures[index], texCoordFlip) * f_Color;

	if(out_color.a < 0.1)
		discard;
}