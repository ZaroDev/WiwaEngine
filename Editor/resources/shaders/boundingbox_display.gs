#version 330 core
layout (line_strip, max_vertices = 26) out;

uniform vec3 u_Min;
uniform vec3 u_Max;
uniform mat4 u_Projection;

const float size = 0.1;

void GeneratePoint(vec3 point)
{
    gl_Position = u_Projection * (vec4(point, 1.0) * size);
    EmitVertex();
}

void main()
{
    //GeneratePoint(vec3(u_Max.x, u_Max.y, u_Max.z));
    //GeneratePoint(vec3(u_Max.x, u_Max.y, u_Min.z));
    //EndPrimitive();
    //GeneratePoint(vec3(u_Max.x, u_Max.y, u_Min.z));
    //GeneratePoint(vec3(u_Min.x, u_Max.y, u_Min.z));
    //EndPrimitive();
} 