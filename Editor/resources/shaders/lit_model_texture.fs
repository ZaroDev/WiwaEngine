#version 330 core
#define MAX_LIGHTS 128

struct TexMaterial {
	sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
// Outputs colors in RGBA
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;


uniform DirLight u_DirLight;
uniform int u_PointLightsNum;
uniform PointLight u_PointLights[MAX_LIGHTS];
uniform vec3 u_ViewPos;
uniform TexMaterial u_Material;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(u_Material.diffuse, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_Material.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, texCoord));
    return (ambient + diffuse + specular);
} 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(u_Material.diffuse, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_Material.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, texCoord));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
	
    return (ambient + diffuse + specular);
} 
void main()
{
	vec3 norm = normalize(normal);
    vec3 viewDir = normalize(u_ViewPos - fragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(u_DirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < u_PointLightsNum; i++)
        result += CalcPointLight(u_PointLights[i], norm, fragPos, viewDir); 
	
	FragColor = vec4(result, 1.0);
}