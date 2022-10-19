#pragma once

#include <Wiwa/Core.h>

#include <vector>
#include <Wiwa/utilities/math/Math.h>

namespace Wiwa {
	class WI_API Model
	{
	protected:
		std::vector<float> vbo_data;
		std::vector<int> ebo_data;

		void generateBuffers();
	private:
		unsigned int vao, vbo, ebo;

		void getMeshFromFile(const char* file);
		
		void CreateCube();
		void CreatePlane();
		void CreatePyramid();
		void CreateSphere();

	public:
		Model(const char* file);
		~Model();

		void Render();
	};
}