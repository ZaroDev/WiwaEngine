#pragma once

#include <Wiwa/core/Core.h>
#include <stdint.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/containers/List.h>
#include <glm/glm.hpp>

namespace Wiwa {
	
	class WI_API FrameBuffer {
	private:
		uint32_t m_FBO;
		uint32_t m_ColorBufferTexture;
		uint32_t m_RBO;
		int m_Width, m_Height;

		bool m_Init;
	public:
		FrameBuffer();
		~FrameBuffer();

		void Init(int width, int height, bool depth=true);

		void Bind(bool clear=true);
		void Unbind();

		void Clear();

		inline int getWidth() { return m_Width; }
		inline int getHeight() { return m_Height; }

		inline uint32_t getColorBufferTexture() { return m_ColorBufferTexture; }
		inline uint32_t getDepthBufferTexture() { return m_RBO; }
	};
}