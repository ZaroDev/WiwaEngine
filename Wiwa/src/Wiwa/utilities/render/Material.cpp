#include "wipch.h"
#include <Wiwa/core/Resources.h>

#include <Wiwa/utilities/json/JSONDocument.h>

namespace Wiwa {

    Material::Material()
        : m_Type(MaterialType::color), m_ResourceId(NULL), m_TextureId(NULL)
    {
        m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

        m_Settings =
        {
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            0.1f
        };
    }

    Material::Material(Color4f color)
        : m_Type(MaterialType::color), m_Color(color), m_ResourceId(NULL), m_TextureId(NULL)
    {
        m_Settings =
        {
            glm::vec3{0.1f, 0.1f, 0.1f},
            glm::vec3{0.1f, 0.1f, 0.1f},
            0.1f
        };
    }

    Material::Material(const char* file)
    {
        Load(file);
    }

    Material::~Material()
    {
    }

    void Material::setTexture(const char* file)
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

    void Material::Load(const char* path)
    {
        JSONDocument matFile(path);

        m_MaterialPath = path;

        m_TexturePath = matFile["texture"].get<const char*>();

        m_Color = {
            matFile["colorR"].get<float>(),
            matFile["colorG"].get<float>(),
            matFile["colorB"].get<float>(),
            matFile["colorA"].get<float>()
        };

        m_Type = (Wiwa::Material::MaterialType)matFile["type"].get<int>();

        m_Settings = {
            { matFile["diffuseR"].get<float>(),  matFile["diffuseG"].get<float>(),  matFile["diffuseB"].get<float>() },
            { matFile["specularR"].get<float>(),  matFile["specularG"].get<float>(),  matFile["specularB"].get<float>() },
            matFile["shininess"].get<float>()
        };

        if (!m_TexturePath.empty())
        {
            m_ResourceId = Resources::Load<Image>(m_TexturePath.c_str());
            m_TextureId = Resources::GetResourceById<Image>(m_ResourceId)->GetTextureId();
        }
    }

    void Material::SaveMaterial(const char* path, Material* mat)
    {
        MaterialSettings& mat_settings = mat->getSettings();

        Wiwa::JSONDocument matFile;
        matFile.AddMember("texture", mat->getTexturePath());
        matFile.AddMember("colorR", mat->getColor().r);
        matFile.AddMember("colorG", mat->getColor().g);
        matFile.AddMember("colorB", mat->getColor().b);
        matFile.AddMember("colorA", mat->getColor().a);
        matFile.AddMember("diffuseR", mat_settings.diffuse.r);
        matFile.AddMember("diffuseG", mat_settings.diffuse.g);
        matFile.AddMember("diffuseB", mat_settings.diffuse.b);
        matFile.AddMember("specularR", mat_settings.specular.r);
        matFile.AddMember("specularG", mat_settings.specular.g);
        matFile.AddMember("specularB", mat_settings.specular.b);
        matFile.AddMember("shininess", mat_settings.shininess);
        matFile.AddMember("type", (int)mat->getType());
        matFile.save_file(path);
    }

    Material* Material::LoadMaterial(const char* path)
    {
        return new Material(path);
    }
}