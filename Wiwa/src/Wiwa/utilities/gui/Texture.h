#pragma once

#include "../../../Core.h"
#include "../../../simple_types.h"

#include "glew.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

#include "../Color.h"

namespace EM {

	class EM_API Texture {
		private:
			uint32 m_TextureId;

			SDL_Surface* m_TexturePixels;

			void createOpenGLTexture();
			void updateOpenGLTexture();
		public:
			Texture();
			~Texture();

			void Load(const char* texture_file);
			void Create(int width, int height, int depth);

			void Update();

			void SetPixel(int x, int y, Color color);
			void SetPixel(int point, Color color);

			uint32 getId() { return m_TextureId; }
	};
}