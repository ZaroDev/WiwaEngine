#version 330 core

in vec2 texCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

out vec4 out_color;

void main()
{
	vec2 texCoordFlip = vec2(texCoord.x, 1.0 - texCoord.y);

	out_color = texture(u_Texture, texCoordFlip) * u_Color;

	if(out_color.a < 0.1)
		discard;
}