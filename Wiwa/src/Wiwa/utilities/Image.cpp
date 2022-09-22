#include "../../headers/utilities/Image.h"

#include <SDL.h>
#include "sdl_image.h"
#include "glew.h"
#include "SDL_opengl.h"

#include <iostream>

EM::Image::Image()
{
	
}

EM::Image::~Image()
{
}

bool EM::Image::Init(const char * path)
{
	SDL_Surface* srfc = IMG_Load(path);

	if (!srfc) {
		return false;
	}

	m_ImageSize.x = srfc->w;
	m_ImageSize.y = srfc->h;

	glGenTextures(1, &m_TextureId);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	
	int PicType = GL_RGBA;
	if (srfc->format->BytesPerPixel == 3)
		PicType = GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, PicType, srfc->w, srfc->h, 0, PicType, GL_UNSIGNED_BYTE, srfc->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(srfc);

	return true;
}
