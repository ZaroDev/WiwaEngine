#include <wipch.h>

#include "Model.h"
#include <fstream>
#include <string>

#include <glew.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm.hpp>

namespace Wiwa {
	//void Mesh::getMeshFromFile(const char * file)
	//{
	//	std::ifstream mesh_file(file, std::ios::in);
	//
	//	if (mesh_file.is_open()) {
	//		std::string line, substrValue;
	//
	//		int dtype, fpos;
	//
	//		while (!mesh_file.eof()) {
	//			std::getline(mesh_file, line);
	//
	//			if (line[0] != '#') {
	//				if (line == "VBO") {
	//					dtype = 0;
	//				}
	//				else if (line == "EBO") {
	//					dtype = 1;
	//				}
	//				else {
	//					if (dtype == 0) {
	//						do {
	//							fpos = line.find(',');
	//
	//							if (fpos != line.npos) {
	//								substrValue = line.substr(0, fpos);
	//
	//								vbo_data.push_back(std::stof(substrValue));
	//
	//								line = line.substr(fpos + 1, line.size());
	//							}
	//						} while (fpos != line.npos);
	//
	//						vbo_data.push_back(std::stof(line));
	//					}
	//					else if (dtype == 1) {
	//						do {
	//							fpos = line.find(',');
	//
	//							if (fpos != line.npos) {
	//								substrValue = line.substr(0, fpos);
	//
	//								ebo_data.push_back(std::stoi(substrValue));
	//
	//								line = line.substr(fpos + 1, line.size());
	//							}
	//						} while (fpos != line.npos);
	//
	//						ebo_data.push_back(std::stoi(line));
	//					}
	//				}
	//			}
	//		}
	//
	//		mesh_file.close();
	//	}
	//}

	void Model::getMeshFromFile(const char* file)
	{
		const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			WI_ERROR("Couldn't load mesh file: {0}", file);
		}
		WI_CORE_INFO("Loading mesh file at: {0} ...", file);
		is_root = true;

		if (scene != nullptr && scene->HasMeshes())
		{
			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
				Model* model = loadmesh(scene->mMeshes[i]);

				models.push_back(model);
			}

			aiReleaseImport(scene);
		}
		else {
			WI_CORE_ERROR("Error loading mesh {0} with error {1}", file, aiGetErrorString());
		}
		m_ModelPath = file;
	}

	Model* Model::loadmesh(const aiMesh* mesh)
	{
		Model* model = new Model(NULL);

		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			// Vertices
			model->vbo_data.push_back(mesh->mVertices[j].x);
			model->vbo_data.push_back(mesh->mVertices[j].y);
			model->vbo_data.push_back(mesh->mVertices[j].z);
			// Normals
			model->vbo_data.push_back(mesh->mNormals[j].x);
			model->vbo_data.push_back(mesh->mNormals[j].y);
			model->vbo_data.push_back(mesh->mNormals[j].z);
			// Texture coordinates
			if (mesh->mTextureCoords[0])
			{
				model->vbo_data.push_back(mesh->mTextureCoords[0][j].x);
				model->vbo_data.push_back(mesh->mTextureCoords[0][j].y);
			}
			else {
				model->vbo_data.push_back(0.0f);
				model->vbo_data.push_back(0.0f);
			}
		}
		if (model->vbo_data.empty())
		{
			WI_CORE_ERROR("Error while loading mesh vertex buffer");
		}
		else
		{
			WI_CORE_INFO("Vertex buffer generated correctly");
		}
		// Indices
		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			aiFace& face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++) {
				model->ebo_data.push_back(face.mIndices[k]);
			}
		}
		if (model->ebo_data.empty())
		{
			WI_CORE_ERROR("Error while loading mesh index buffer");
		}
		else
		{
			WI_CORE_INFO("Index buffer generated correctly");
		}
		model->generateBuffers();

		return model;
	}

	void Model::CreateCube()
	{
		vbo_data = {
			1,1,1, 0,0,1, 0.625,0.5,
			-1,1,1, 0,0,1, 0.875,0.5,
			-1,-1,1, 0,0,1, 0.875,0.25,
			1,-1,1, 0,0,1, 0.625,0.25,
			1,-1,-1, 0,-1,0, 0.375,0.25,
			1,-1,1, 0,-1,0, 0.625,0.25,
			-1,-1,1, 0,-1,0, 0.625,0,
			-1,-1,-1, 0,-1,0, 0.375,0,
			-1,-1,-1, -1,0,0, 0.375,1,
			-1,-1,1, -1,0,0, 0.625,1,
			-1,1,1, -1,0,0, 0.625,0.75,
			-1,1,-1, -1,0,0, 0.375,0.75,
			-1,1,-1, 0,0,-1, 0.125,0.5,
			1,1,-1, 0,0,-1, 0.375,0.5,
			1,-1,-1, 0,0,-1, 0.375,0.25,
			-1,-1,-1, 0,0,-1, 0.125,0.25,
			1,1,-1, 1,0,0, 0.375,0.5,
			1,1,1, 1,0,0, 0.625,0.5,
			1,-1,1, 1,0,0, 0.625,0.25,
			1,-1,-1, 1,0,0, 0.375,0.25,
			-1,1,-1, 0,1,0, 0.375,0.75,
			-1,1,1, 0,1,0, 0.625,0.75,
			1,1,1,	0,1,0, 0.625,0.5,
			1,1,-1, 0,1,0, 0.375,0.5
		};

		ebo_data = {
			0,1,2,0,2,3,
			4,5,6,4,6,7,
			8,9,10,8,10,11,
			12,13,14,12,14,15,
			16,17,18,16,18,19,
			20,21,22,20,22,23
		};

		generateBuffers();
	}

	void Model::CreatePlane()
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

	void Model::CreatePyramid()
	{
		vbo_data = {
			-0.25,0.00,-0.25,
			0.25,0.00,-0.25,
			-0.25,0.00,0.25,
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

	void Model::CreateSphere()
	{
		int stack_count = 18;
		int sector_count = 36;
		int radius = 1;

		float x, y, z, xy;
		float sectorStep = 2 * PI_F / sector_count;
		float stackStep = PI_F / stack_count;
		float sectorAngle, stackAngle;

		// Generate vertices
		for (int i = 0; i <= stack_count; ++i)
		{
			stackAngle = (float)PI / 2 - i * stackStep;
			xy = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);

			for (int j = 0; j <= sector_count; ++j)
			{
				sectorAngle = j * sectorStep;

				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);
				vbo_data.push_back(x);
				vbo_data.push_back(y);
				vbo_data.push_back(z);
			}
		}

		// Generate indices
		int k1, k2;
		for (int i = 0; i < stack_count; ++i)
		{
			k1 = i * (sector_count + 1);     // beginning of current stack
			k2 = k1 + sector_count + 1;      // beginning of next stack

			for (int j = 0; j < sector_count; ++j, ++k1, ++k2)
			{
				if (i != 0)
				{
					ebo_data.push_back(k1);
					ebo_data.push_back(k2);
					ebo_data.push_back(k1 + 1);
				}

				if (i != (stack_count - 1))
				{
					ebo_data.push_back(k1 + 1);
					ebo_data.push_back(k2);
					ebo_data.push_back(k2 + 1);
				}
			}
		}

		generateBuffers();
	}

	void Model::generateBuffers()
	{
		WI_CORE_INFO("Generating buffers...");
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);
		WI_CORE_INFO("Generating buffers DONE");
		
		WI_CORE_INFO("Binding the vertex array ...");
		glBindVertexArray(vao);
		WI_CORE_INFO("Binding the vertex array DONE");

		WI_CORE_INFO("Binding the vertex buffer ...");
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(float), vbo_data.data(), GL_STATIC_DRAW);
		WI_CORE_INFO("Binding the vertex buffer DONE");

		WI_CORE_INFO("Binding the index buffer ...");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_data.size() * sizeof(int), ebo_data.data(), GL_STATIC_DRAW);
		WI_CORE_INFO("Binding the index buffer DONE");

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	Model::Model(const char* file)
		: ebo(0), vbo(0), vao(0)
	{
		is_root = true;

		if (file) {
			if (strcmp(file, "cube") == 0)
			{
				CreateCube();
			}
			else if (strcmp(file, "plane") == 0)
			{
				CreatePlane();
			}
			else if (strcmp(file, "pyramid") == 0)
			{
				CreatePyramid();
			}
			else if (strcmp(file, "sphere") == 0)
			{
				CreateSphere();
			}
			else
			{
				getMeshFromFile(file);
				generateBuffers();
			}
		}
		else {
			is_root = false;
		}
	}

	Model::~Model()
	{
	}

	void Model::Render()
	{
		if(is_root){
			size_t meshCount = models.size();

			for (size_t i = 0; i < meshCount; i++) {
				models[i]->Render();
			}
		}
		else {
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, (GLsizei)ebo_data.size(), GL_UNSIGNED_INT, 0);
		}
	}
}