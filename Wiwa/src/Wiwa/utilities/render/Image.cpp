#include <wipch.h>
#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define CHECKERS_HEIGHT 100
#define CHECKERS_WIDTH 100

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
	bool Image::CreateCheckerText()
	{
		GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
		for (int i = 0; i < CHECKERS_HEIGHT; i++) {
			for (int j = 0; j < CHECKERS_WIDTH; j++) {
				int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
				checkerImage[i][j][0] = (GLubyte)c;
				checkerImage[i][j][1] = (GLubyte)c;
				checkerImage[i][j][2] = (GLubyte)c;
				checkerImage[i][j][3] = (GLubyte)255;
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &m_TextureId);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
			0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

		return true;
	}
}