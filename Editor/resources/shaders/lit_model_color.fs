#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec4 u_Color;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform Material u_Material;

float ambientSrenght = 0.1;
float specularStrenght = 0.5;

void main() {
	vec3 ambient = u_Material.ambient * u_LightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(u_LightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * u_Material.diffuse) * u_LightColor;


	vec3 viewDir = normalize(u_ViewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec= pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specular = (u_Material.specular * spec) * u_LightColor;

	vec3 lightResult = (ambient + diffuse + specular);
	FragColor = vec4(lightResult, 1.0) * u_Color;
}