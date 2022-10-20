#include "wipch.h"
#include <Wiwa/Resources.h>

#include <Wiwa/utilities/json/JSONDocument.h>

Wiwa::Material::Material()
	: m_Type(MaterialType::color), m_ResourceId(NULL), m_TextureId(NULL)
{
	m_Color = { 0.2, 0.2, 0.2, 1.0 };
}

Wiwa::Material::Material(Color4f color)
	: m_Type(MaterialType::color), m_Color(color), m_ResourceId(NULL), m_TextureId(NULL)
{
}

Wiwa::Material::Material(const char* file)
{
    JSONDocument matFile(file);

    std::string texturePath = matFile["texture"].get<const char*>();
    m_Color = {
        matFile["colorR"].get<float>(),
        matFile["colorG"].get<float>(),
        matFile["colorB"].get<float>(),
        matFile["colorA"].get<float>()
    };
    m_Type = (Wiwa::Material::MaterialType)matFile["type"].get<int>();
    if (!texturePath.empty())
    {
        m_ResourceId = Resources::Load<Image>(texturePath.c_str());
        m_TextureId = Resources::GetResourceById<Image>(m_ResourceId)->GetTextureId();
    }
}

Wiwa::Material::~Material()
{
}

void Wiwa::Material::setTexture(const char* file)
{
	m_ResourceId = Resources::Load<Image>(file);
	m_TextureId = Resources::GetResourceById<Image>(m_ResourceId)->GetTextureId();
}
