#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 26) out;

const float MAGNITUDE = 0.4;
  
uniform mat4 u_Projection;
uniform vec3 u_Min;
uniform vec3 u_Max;

void GeneratePoint(vec3 position)
{
    gl_Position = u_Projection * vec4(position, 1.0);
    EmitVertex();
}

void main()
{
    GeneratePoint(vec3(u_Max.x, u_Max.y, u_Max.z));    
    GeneratePoint(vec3(u_Max.x, u_Max.y, u_Min.z));
    EndPrimitive();
  
}  