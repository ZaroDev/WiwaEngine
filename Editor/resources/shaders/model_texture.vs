#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 o_texCoor;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(v_pos, 1.0);
    o_texCoor = v_texCoord;
}