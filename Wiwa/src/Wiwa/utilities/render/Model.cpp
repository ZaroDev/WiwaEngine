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
		const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
		if (scene != nullptr && scene->HasMeshes())
		{
			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
				for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
					vbo_data.push_back(scene->mMeshes[i]->mVertices[j].x);
					vbo_data.push_back(scene->mMeshes[i]->mVertices[j].y);
					vbo_data.push_back(scene->mMeshes[i]->mVertices[j].z);
					vbo_data.push_back(scene->mMeshes[i]->mNormals[j].x);
					vbo_data.push_back(scene->mMeshes[i]->mNormals[j].y);
					vbo_data.push_back(scene->mMeshes[i]->mNormals[j].z);
					if (scene->mMeshes[i]->mTextureCoords[0])
					{
						vbo_data.push_back(scene->mMeshes[i]->mTextureCoords[0][j].x);
						vbo_data.push_back(scene->mMeshes[i]->mTextureCoords[0][j].y);
					}
				}


				for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
					for (unsigned int k = 0; k < scene->mMeshes[i]->mFaces[j].mNumIndices; k++) {
						ebo_data.push_back(scene->mMeshes[i]->mFaces[j].mIndices[k]);
					}
				}

			}

			aiReleaseImport(scene);
		}
		else {
			WI_CORE_ERROR("Error loading mesh {0} with error {1}", file, aiGetErrorString());
		}
	}

	void Model::CreateCube()
	{
		vbo_data = {
			// FACE 1
			-0.5,-0.5,-0.5,
			0.5,-0.5,-0.5,
			0.5,0.5,-0.5,
			0.5,0.5,-0.5,
			-0.5,0.5,-0.5,
			-0.5,-0.5,-0.5,
			// FACE 3
			-0.5,-0.5,0.5,
			0.5,-0.5,0.5,
			0.5,0.5,0.5,
			0.5,0.5,0.5,
			-0.5,0.5,0.5,
			-0.5,-0.5,0.5,
			// Face 3
			-0.5,0.5,0.5,
			-0.5,0.5,-0.5,
			-0.5,-0.5,-0.5,
			-0.5,-0.5,-0.5,
			-0.5,-0.5,0.5,
			-0.5,0.5,0.5,
			// Face 4
			0.5,0.5,0.5,
			0.5,0.5,-0.5,
			0.5,-0.5,-0.5,
			0.5,-0.5,-0.5,
			0.5,-0.5,0.5,
			0.5,0.5,0.5,
			// Face 5
			-0.5,-0.5,-0.5,
			0.5,-0.5,-0.5,
			0.5,-0.5,0.5,
			0.5,-0.5,0.5,
			-0.5,-0.5,0.5,
			-0.5,-0.5,-0.5,
			// Face 6
			-0.5,0.5,-0.5,
			0.5,0.5,-0.5,
			0.5,0.5,0.5,
			0.5,0.5,0.5,
			-0.5,0.5,0.5,
			-0.5,0.5,-0.5
		};

		ebo_data = {
			0,1,2,3,4,5,
			8,7,6,11,10,9,
			14,13,12,17,16,15,
			18,19,20,21,22,23,
			26,25,24,29,28,27,
			30,31,32,33,34,35
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
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(float), vbo_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_data.size() * sizeof(int), ebo_data.data(), GL_STATIC_DRAW);

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
		else if (file) 
		{
			getMeshFromFile(file);
			generateBuffers();
		}
	}

	Model::~Model()
	{
	}

	void Model::Render()
	{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, (GLsizei)ebo_data.size(), GL_UNSIGNED_INT, 0);
	}
}