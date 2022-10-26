#version 330 core

struct TexMaterial {
	sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
// Outputs colors in RGBA
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform vec3 u_LightColor;

uniform vec3 u_ViewPos;
uniform TexMaterial u_Material;
uniform Light u_Light;

void main()
{
	vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, texCoord));

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-u_Light.position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Light.diffuse * diff * vec3(texture(u_Material.diffuse, texCoord)); 


	vec3 viewDir = normalize(u_ViewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec= pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess); 
	vec3 specular = vec3(texture(u_Material.specular, texCoord)) * spec * u_Light.specular;

	vec3 lightResult = (ambient + diffuse + specular);
	
	FragColor = vec4(lightResult, 1.0);
}