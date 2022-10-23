#pragma once

#include <Wiwa/Core.h>
#include <Wiwa/utilities/math/Vector4f.h>
#include <Wiwa/utilities/math/Vector2i.h>

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
		Material();
		Material(Color4f color);
		Material(const char* file);
		~Material();

		inline MaterialType getType() { return m_Type; }
		inline uint32_t getTextureId() { return m_TextureId; }
		inline uint32_t getResourceId() { return (uint32_t)m_ResourceId; }
		inline Color4f getColor() { return m_Color; }
		inline const char* getMaterialPath() { return m_MaterialPath.c_str(); }
		inline const char* getTexturePath() { return m_TexturePath.c_str(); }

		void setTexture(const char* file);
		inline void setColor(Color4f color) { m_Color = color; }
		inline void setType(MaterialType type) { m_Type = type; }
		inline Size2i GetTextureSize() { return m_TextureSize; }
	private:
		ResourceId m_ResourceId;
		std::string m_TexturePath;
		std::string m_MaterialPath;
		uint32_t m_TextureId;
		Size2i m_TextureSize;
		Color4f m_Color;
		MaterialType m_Type;
	};
}