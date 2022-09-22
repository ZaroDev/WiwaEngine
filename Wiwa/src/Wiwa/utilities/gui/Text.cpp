#include "../../../headers/utilities/gui/Text.h"

#include <glew.h>
#include <SDL_opengl.h>

Text::Text()
{
	mRenderer2D = Renderer2D::getInstance();

	mColor = { 255, 255, 255 };
	glGenTextures(1, &mTexture);
}

Text::~Text()
{
}

void Text::setFont(const char * font)
{
	mFont = Resources::getInstance()->Load<Font>(font);
}

void Text::setFont(ResourceId font)
{
	mFont = font;
}

void Text::setText(const char * text)
{
	Font* font = Resources::getInstance()->GetResourceById<Font>(mFont);
	TTF_Font* fobj = font->getFontObject();

	SDL_Color color = { 255, 255, 255 };

	SDL_Surface* srfc = TTF_RenderText_Solid(fobj, text, color);

	// Generate the texture
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, srfc->w, srfc->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, srfc->pixels);

	// Generate mip maps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free surface
	SDL_FreeSurface(srfc);
}

void Text::setColor(int red, int green, int blue)
{
	mColor = { red, green, blue };
}

void Text::setColor(Color color)
{
	mColor = color;
}

void Text::render()
{
	mRenderer2D->RenderQuad(mTexture, mColor);
}
