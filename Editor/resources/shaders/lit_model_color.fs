#version 330 core

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec4 u_Color;
uniform DirLight u_DirLight;
uniform vec3 u_ViewPos;
uniform Material u_Material;



void main() {
	vec3 lightDir = normalize(-u_DirLight.direction);
	vec3 ambient = u_Material.diffuse * u_DirLight.ambient;
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * u_Material.diffuse) * u_DirLight.diffuse;


	vec3 viewDir = normalize(u_ViewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec= pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specular = (u_Material.specular * spec) * u_DirLight.specular;

	vec3 lightResult = (ambient + diffuse + specular);
	FragColor = vec4(lightResult, 1.0) * u_Color;
}