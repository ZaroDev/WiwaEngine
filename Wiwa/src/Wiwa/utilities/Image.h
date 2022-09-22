#pragma once

#include "Empathy/Core.h"
#include "Empathy/simple_types.h"
#include "Empathy/headers/math/Vector2i.h"

namespace EM {
	class EM_API Image {
	private:
		uint32 m_TextureId;
		Size2i m_ImageSize;
	public:
		Image();
		~Image();

		bool Init(const char* path);

		uint32 GetTextureId() { return m_TextureId; }
		Size2i GetSize() { return m_ImageSize; }
	};
}