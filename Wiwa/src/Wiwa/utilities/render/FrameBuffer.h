#pragma once

#include <Wiwa/Core.h>
#include <stdint.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <glm.hpp>

namespace Wiwa {
	struct Light
	{
		glm::vec3 Position;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
	};
	class WI_API FrameBuffer {
	private:
		uint32_t m_FBO;
		uint32_t m_ColorBufferTexture;
		uint32_t m_RBO;
		int m_Width, m_Height;
		Light m_Light;

	public:
		void Init(int width, int height);

		void Bind(bool clear=true);
		void Unbind();

		void Clear();

		inline int getWidth() { return m_Width; }
		inline int getHeight() { return m_Height; }

		inline uint32_t getColorBufferTexture() { return m_ColorBufferTexture; }

		inline void setLight(const Light& light) { m_Light = light; }

		inline Light& getLight() { return m_Light; }
	};
}