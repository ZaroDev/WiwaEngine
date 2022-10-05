#include <wipch.h>
#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"
#include <glew.h>

namespace Wiwa {
	Image::Image()
	{

	}

	Image::~Image()
	{
	}

	bool Image::Init(const char* path)
	{
		int w, h, ch;

		unsigned char* image = stbi_load(path, &w, &h, &ch, STBI_rgb_alpha);

		if (!image) {
			return false;
		}

		glGenTextures(1, &m_TextureId);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);

		int PicType = GL_RGBA;
		/*if (ch == 3)
			PicType = GL_RGB;*/
		glTexImage2D(GL_TEXTURE_2D, 0, PicType, w, h, 0, PicType, GL_UNSIGNED_BYTE, image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);

		m_ImageSize.x = w;
		m_ImageSize.y = h;
		m_ImageChannels = ch;

		return true;
	}
}