#pragma once

#include "../../../Core.h"

#include "../../Resources.h"
#include "../../../simple_types.h"

#include "../Color.h"

#include "../../Renderer2D.h"

class EM_API Text {
	private:
		ResourceId mFont;
		Color mColor;

		uint32 mTexture;

		Renderer2D* mRenderer2D;
	public:
		Text();
		~Text();

		void setFont(const char* font);
		void setFont(ResourceId font);
		void setText(const char* text);

		void setColor(int red, int green, int blue);
		void setColor(Color color);

		void render();
};