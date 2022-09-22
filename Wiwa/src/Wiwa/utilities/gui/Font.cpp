#include "../../../headers/utilities/gui/Font.h"

Font::Font(const char* font_file)
{
	mFont = TTF_OpenFont(font_file, 25);
}

Font::~Font()
{
	TTF_CloseFont(mFont);
}
