#version 330 core

out vec4 o_Color;

in vec2 s_TexCoord;

// Texture
uniform sampler2D u_LayerTex;

void main()
{
	// Flip texture in Y axis
	vec2 tex_flip = s_TexCoord;
	tex_flip.y = 1.0 - tex_flip.y;
	
	// Output color
	o_Color = texture(u_LayerTex, tex_flip);
	
	// Discard alpha pixels
	if(o_Color.a < 0.1)
		discard;
}