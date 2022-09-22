#pragma once

#include <SDL_mixer.h>
#include "../../simple_types.h"
#include "../../Core.h"

struct EM_API AudioClip {

	// Mix chunk
	Mix_Chunk* chunk;

	// Duration in seconds
	uint32 duration;
};