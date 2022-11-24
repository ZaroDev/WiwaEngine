#pragma once

#include <Wiwa/core/Core.h>

#include <vector>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/math/AABB.h>

struct aiMesh;

namespace Wiwa {
	class WI_API Model
	{
	private:
		std::string m_ModelPath;
	protected:
		bool is_root = false;

		std::vector<float> vbo_data;
		std::vector<int> ebo_data;
		std::vector<int> bbebo_data;
		std::vector<float> bbvbo_data;

		std::vector<Model*> models;
		void generateBuffers();

	private:
		unsigned int vao, vbo, ebo, bbvao, bbvbo, bbebo;

		void getMeshFromFile(const char* file);

		Model* loadmesh(const aiMesh* mesh);
		
		void CreateCube();
		void CreatePlane();
		void CreatePyramid();
		void CreateSphere();
	public:
		Model(const char* file);
		~Model();

		void Render();

		void DrawBoudingBox();

		inline unsigned int getVAO() { return vao; }

		inline const char* getModelPath() { return m_ModelPath.c_str(); }
	public:
		bool showNormals = false;
		AABB boundingBox;
	};
}