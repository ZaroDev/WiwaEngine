#pragma once

#include <vector>

#include <Wiwa/utilities/math/Math.h>

namespace Wiwa {
	class Mesh
	{
	protected:
		std::vector<float> vbo_data;
		std::vector<int> ebo_data;

		void generateBuffers();
	private:
		unsigned int vao, vbo, ebo;

		void getMeshFromFile(const char* file);
		
	public:
		Mesh(const char* file);
		~Mesh();

		void Render();
	};
}