#pragma once

#include <Wiwa/Core.h>
#include <Wiwa/utilities/math/Vector4f.h>

namespace Wiwa {
	typedef size_t ResourceId;

	class WI_API Material
	{
	public:
		enum MaterialType
		{
			textured,
			color
		};
		Material();
		Material(Color4f color);
		Material(const char* file);
		~Material();

		inline MaterialType getType() { return m_Type; }
		inline uint32_t getTextureId() { return m_TextureId; }
		inline uint32_t getResourceId() { return m_ResourceId; }
		inline Color4f getColor() { return m_Color; }

		void setTexture(const char* file);
		inline void setColor(Color4f color) { color = m_Color; }
	private:
		ResourceId m_ResourceId;
		uint32_t m_TextureId;
		Color4f m_Color;
		MaterialType m_Type;
	};
}