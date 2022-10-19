#pragma once

#include <Wiwa/Core.h>

#include <vector>
#include <Wiwa/utilities/math/Math.h>

namespace Wiwa {
	class WI_API Model
	{
	protected:
		std::vector<float> vbo_data;
		std::vector<uint16_t> ebo_data;

		void generateBuffers();
	private:
		unsigned int vao, vbo, ebo;

		void getMeshFromFile(const char* file);
		
		void CreateCube();
		void CreatePlane();
		void CreatePyramid();
		void CreateSphere();


	public:
		Model();
		Model(const char* file);
		~Model();

		void Render();

		inline float* GetVerticies() { return vbo_data.data(); }
		inline uint16_t* GetIndicies() { return ebo_data.data(); }

		//Need to refactor | rethink
		inline void AddVertex(float vertex) { vbo_data.push_back(vertex); }
		inline void AddIndex(uint32_t index) { ebo_data.push_back(index); }
		inline void ClearEBO() { ebo_data.clear(); }
		inline void ClearVBO() { vbo_data.clear(); }
		
		
		inline size_t GetNumVertex() { return vbo_data.size(); }
		inline size_t GetNumIndicies() { return ebo_data.size(); }
	};
}