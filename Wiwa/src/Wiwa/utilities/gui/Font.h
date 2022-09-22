#pragma once

#include "../../../Core.h"

#include <SDL_ttf.h>

class Font {
	private:
		TTF_Font* mFont;
	public:
		Font(const char* font_file);
		~Font();

		TTF_Font* getFontObject() { return mFont; }
};