#pragma once

#include <Wiwa/Core.h>
#include <stdint.h>

namespace Wiwa {
	class WI_API FrameBuffer {
	private:
		uint32_t m_FBO;
		uint32_t m_ColorBufferTexture;
		// Future render buffer object for depth
		//uint32 m_RBO;
		int m_Width, m_Height;
	public:
		void Init(int width, int height);

		void Bind();
		void Unbind();

		int getWidth() { return m_Width; }
		int getHeight() { return m_Height; }

		uint32_t getColorBufferTexture() { return m_ColorBufferTexture; }
	};
}