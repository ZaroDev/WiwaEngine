#include "wipch.h"
#include <Wiwa/core/Resources.h>

#include <Wiwa/utilities/json/JSONDocument.h>

Wiwa::Material::Material()
	: m_Type(MaterialType::color), m_ResourceId(NULL), m_TextureId(NULL)
{
	m_Color = { 0.2f, 0.2f, 0.2f, 1.0f};
    m_Settings = 
    {
        glm::vec3{0.1f, 0.1f, 0.1f},
        glm::vec3{0.1f, 0.1f, 0.1f},
        0.1f
    };
}

Wiwa::Material::Material(Color4f color)
	: m_Type(MaterialType::color), m_Color(color), m_ResourceId(NULL), m_TextureId(NULL)
{
    m_Settings =
    {
        glm::vec3{0.1f, 0.1f, 0.1f},
        glm::vec3{0.1f, 0.1f, 0.1f},
        0.1f
    };
}

Wiwa::Material::Material(const char* file)
{
    JSONDocument matFile(file);
    m_MaterialPath = file;
    m_TexturePath = matFile["texture"].get<const char*>();
    m_Color = {
        matFile["colorR"].get<float>(),
        matFile["colorG"].get<float>(),
        matFile["colorB"].get<float>(),
        matFile["colorA"].get<float>()
    };
    m_Type = (Wiwa::Material::MaterialType)matFile["type"].get<int>();
    m_Settings = {
        { matFile["diffuseR"].get<float>(),  matFile["diffuseG"].get<float>(),  matFile["diffuseB"].get<float>()},
        { matFile["specularR"].get<float>(),  matFile["specularG"].get<float>(),  matFile["specularB"].get<float>()},
        matFile["shininess"].get<float>()
    };
    if (!m_TexturePath.empty())
    {
        m_ResourceId = Resources::Load<Image>(m_TexturePath.c_str());
        m_TextureId = Resources::GetResourceById<Image>(m_ResourceId)->GetTextureId();
    }
}

Wiwa::Material::~Material()
{
}

void Wiwa::Material::setTexture(const char* file)
{
    if (strcmp(file, "checker") == 0)
    {
        Image* img = new Image();
        img->CreateCheckerText();
        m_TextureId = img->GetTextureId();
        return;
    }
    m_TexturePath = file;
	m_ResourceId = Resources::Load<Image>(file);
	Image* img = Resources::GetResourceById<Image>(m_ResourceId);
    m_TextureId = img->GetTextureId();
    m_TextureSize = img->GetSize();
}
