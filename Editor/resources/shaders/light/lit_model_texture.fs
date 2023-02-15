#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 u_BaseLightColor;
uniform vec3 u_BaseLightIntensity;

uniform vec3 u_MaterialAmbientColor;

uniform sampler2D u_Texture;

void main()
{
    FragColor = texture2D(u_Texture, TexCoord.xy) *
            vec4(u_MaterialAmbientColor, 1.0f) *
            vec4(u_BaseLightColor, 1.0f) *
            u_BaseLightIntensity;
}