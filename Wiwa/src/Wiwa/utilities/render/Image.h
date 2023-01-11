#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector2i.h>

#include <stdint.h>

namespace Wiwa {
	class WI_API Image {
	private:
		uint32_t m_TextureId;
		Size2i m_ImageSize;
		int m_ImageChannels;
	public:
		Image();
		~Image();

		bool Init(const char* path);
		bool InitDDS(const char* path);

		bool CreateCheckerText();

		uint32_t GetTextureId() { return m_TextureId; }
		int GetChannels() { return m_ImageChannels; }

		int GetWidth() { return m_ImageSize.w; }
		int GetHeight() { return m_ImageSize.h; }
		Size2i GetSize() { return m_ImageSize; }

		// Image utils
		static void raw_to_dds_file(const char* filename, const unsigned char* pData, int width, int height, int bpp);
		static unsigned char* raw_to_dds_mem(const unsigned char* data, int width, int height, int bpp, size_t* outSize);
		static void free_dds_mem(unsigned char* data);
	};
}