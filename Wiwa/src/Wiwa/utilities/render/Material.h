#pragma once

#include <Wiwa/Core.h>
#include <Wiwa/utilities/math/Vector4f.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/math/Vector2i.h>

#include <glm.hpp>

namespace Wiwa {
	typedef size_t ResourceId;

	class WI_API Material
	{
	public:
		enum MaterialType
		{
			color,
			textured
		};
		struct MaterialSettings
		{
			glm::vec3 diffuse;
			glm::vec3 specular;
			float shininess;
		};

		Material();
		Material(Color4f color);
		Material(const char* file);
		~Material();

		inline MaterialType getType() { return m_Type; }
		inline uint32_t getTextureId() { return m_TextureId; }
		inline uint32_t getSpecularId() { return m_SpecularId; }
		inline uint32_t getResourceTexId() { return (uint32_t)m_ResourceTexId; }
		inline uint32_t getResourceSpecId() { return (uint32_t)m_ResourceSpecId; }
		inline Color4f getColor() { return m_Color; }
		inline const char* getMaterialPath() { return m_MaterialPath.c_str(); }
		inline const char* getTexturePath() { return m_TexturePath.c_str(); }
		inline Size2i &getTextureSize() { return m_TextureSize; }
		inline MaterialSettings& getSettings() { return m_Settings; }

		void setTexture(const char* file);
		void setSpecular(const char* file);
		inline void setColor(Color4f color) { m_Color = color; }
		inline void setType(MaterialType type) { m_Type = type; }
		inline void setSettings(const MaterialSettings& settings) { m_Settings = settings; }

	private:
		ResourceId m_ResourceTexId;
		ResourceId m_ResourceSpecId;
		std::string m_TexturePath;
		std::string m_SpecularPath;
		std::string m_MaterialPath;
		
		uint32_t m_TextureId;
		uint32_t m_SpecularId;
		Size2i m_TextureSize = { 0,0 };
		
		MaterialSettings m_Settings;
		MaterialType m_Type;
		Color4f m_Color;
	};
}