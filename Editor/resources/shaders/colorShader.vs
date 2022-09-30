#version 330 core

layout (location = 0) in vec3 l_VPos;
layout (location = 1) in vec2 l_PosOffset;
layout (location = 2) in vec2 l_Scale;
layout (location = 3) in vec4 l_Color;

//uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec4 f_Color;

void main()
{
	mat4 idmatrix = mat4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	);
    //gl_Position = projection * view * model * vec4(l_VPos, 1.0);
	// Scale
	vec3 vpos = l_VPos;
	vpos.xy = vpos.xy * l_Scale.xy;
	// Translate
	vpos.xy += l_PosOffset;
	gl_Position = u_Proj * u_View * idmatrix * vec4(vpos, 1.0);
	f_Color = l_Color;
}