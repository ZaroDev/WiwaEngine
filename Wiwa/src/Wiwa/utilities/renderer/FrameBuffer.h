#pragma once

#include "Empathy/simple_types.h"
#include "Empathy/Core.h"

namespace EM {
	class EM_API FrameBuffer {
	private:
		uint32 m_FBO;
		uint32 m_ColorBufferTexture;
		// Future render buffer object for depth
		//uint32 m_RBO;
	public:
		void Init();
		void Bind();
		void Unbind();

		uint32 getColorBufferTexture() { return m_ColorBufferTexture; }
	};
}