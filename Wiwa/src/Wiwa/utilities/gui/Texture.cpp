#include "../../../headers/utilities/gui/Texture.h"

void EM::Texture::createOpenGLTexture()
{
	glGenTextures(1, &m_TextureId);

	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_TexturePixels->w, m_TexturePixels->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TexturePixels->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void EM::Texture::updateOpenGLTexture()
{

}

EM::Texture::Texture()
{
}

EM::Texture::~Texture()
{
}

void EM::Texture::Load(const char * texture_file)
{
	m_TexturePixels = IMG_Load(texture_file);

	createOpenGLTexture();
}

void EM::Texture::Create(int width, int height, int depth)
{
	m_TexturePixels = SDL_CreateRGBSurface(0, width, height, depth, 0, 0, 0, 0);

	createOpenGLTexture();
}

void EM::Texture::Update()
{
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_TexturePixels->w, m_TexturePixels->h, GL_RGB, GL_UNSIGNED_BYTE, m_TexturePixels->pixels);
}

void EM::Texture::SetPixel(int x, int y, Color color)
{
	uint32* pixels = (uint32*)m_TexturePixels->pixels;

	pixels[y * m_TexturePixels->w + x] = color.ToByte32();
}

void EM::Texture::SetPixel(int point, Color color)
{
	uint32* pixels = (uint32*)m_TexturePixels->pixels;

	pixels[point] = color.ToByte32();
}