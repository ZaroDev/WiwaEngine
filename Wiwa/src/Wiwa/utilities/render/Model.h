#pragma once

#include <Wiwa/core/Core.h>

#include <vector>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/math/AABB.h>

struct aiMesh;
struct aiNode;

namespace Wiwa {
	struct ModelHierarchy {
		std::string name;

		// Index in model list
		std::vector<unsigned int> meshIndexes;

		// Model children
		std::vector<ModelHierarchy*> children;

		~ModelHierarchy() {
			size_t len = children.size();

			for (size_t i = 0; i < len; i++) {
				delete children[i];
			}

			children.clear();
			meshIndexes.clear();
		}
	};

	class WI_API Model
	{
	private:
		std::string m_ModelPath;
	protected:
		bool is_root = false;

		std::string model_name;

		std::vector<float> vbo_data;
		std::vector<int> ebo_data;
		std::vector<int> bbebo_data;
		std::vector<float> bbvbo_data;

		std::vector<Model*> models;
		ModelHierarchy* model_hierarchy;

		void generateBuffers();
	private:
		unsigned int vao, vbo, ebo, bbvao, bbvbo, bbebo;

		void getMeshFromFile(const char* file, bool gen_buffers=true);
		void getWiMeshFromFile(const char* file);

		Model* loadmesh(const aiMesh* mesh);
		ModelHierarchy* loadModelHierarchy(const aiNode* node);
		
		void CreateCube();
		void CreatePlane();
		void CreatePyramid();
		void CreateSphere();
	public:
		Model(const char* file);
		~Model();

		void Render();

		void DrawBoudingBox();

		unsigned int getVAO() { return vao; }

		const char* getModelPath() { return m_ModelPath.c_str(); }

		Model* getModelAt(size_t index) { return models[index]; }

		const ModelHierarchy* getModelHierarchy() { return model_hierarchy; }
		std::string getModelName() { return model_name; }

		void LoadMesh(const char* file);
		void LoadWiMesh(const char* file);

		static Model* GetModelFromFile(const char* file);
		static void SaveModel(Model* model, const char* file);
	public:
		bool showNormals = false;
		AABB boundingBox;
	};
}