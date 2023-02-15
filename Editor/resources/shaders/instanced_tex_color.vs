#version 330 core

// Vertex data
layout (location = 0) in vec3 l_VPos;
layout (location = 1) in vec2 l_TexCoord;

// Instanced data
layout (location = 2) in vec2 l_PosOffset;
layout (location = 3) in vec2 l_Scale;
layout (location = 4) in vec4 l_Color;
layout (location = 5) in float l_TexID;
layout (location = 6) in vec2 l_TexClip[4];

//uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec4 f_Color;
out vec2 f_TexCoord;
out float f_TexID;

void main()
{	
	// Scale
	vec3 vpos = l_VPos;
	vpos.xy = vpos.xy * l_Scale.xy;
	
	// Translate
	vpos.xy += l_PosOffset;
	
	// Out position
	gl_Position = u_Proj * u_View * vec4(vpos, 1.0);
	
	f_Color = l_Color;
	f_TexCoord = l_TexClip[gl_VertexID];
	f_TexID = l_TexID;
}