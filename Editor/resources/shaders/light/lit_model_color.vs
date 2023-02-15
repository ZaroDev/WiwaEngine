#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
    fragPos = vec3(u_Model * vec4(aPos, 1.0));
	//TODO: Compute at the CPU!
	normal = mat3(transpose(inverse(u_Model))) * aNormal;  
}