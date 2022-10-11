#include <wipch.h>

#include "Pyramid.h"

namespace Wiwa {
	Pyramid::Pyramid() : Mesh(NULL)
	{
		vbo_data = {
			-0.25,0.00,-0.25,
			0.25,0.00,-0.25,
			- 0.25,0.00,0.25,
			0.25,0.00,0.25,
			0.00,0.50,0.00
		};

		ebo_data = {
			0,2,3,
			0,1,3,
			0,1,4,
			1,3,4,
			3,2,4,
			2,0,4
		};

		generateBuffers();
	}

	Pyramid::~Pyramid()
	{
	}
}