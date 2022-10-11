#include <wipch.h>

#include "Plane.h"

namespace Wiwa {
	Plane::Plane() : Mesh(NULL)
	{
		vbo_data = {
			0.5f, 0.0f, 0.5f,
			0.5f, 0.0f, -0.5f,
			-0.5f, 0.0f, -0.5f,
			-0.5f, 0.0f, 0.5f
		};

		ebo_data = {
			0, 1, 3, 1, 2, 3
		};

		generateBuffers();
	}

	Plane::~Plane()
	{
	}
}