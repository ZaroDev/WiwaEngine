#include <wipch.h>

#include "Mesh.h"
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

	void Mesh::getMeshFromFile(const char* file)
	{
		const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
		if (scene != nullptr && scene->HasMeshes())
		{
			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
				for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
					vbo_data.push_back(scene->mMeshes[i]->mVertices[j].x);
					vbo_data.push_back(scene->mMeshes[i]->mVertices[j].y);
					vbo_data.push_back(scene->mMeshes[i]->mVertices[j].z);
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

	void Mesh::generateBuffers()
	{
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(float), vbo_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_data.size() * sizeof(int), ebo_data.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	Mesh::Mesh(const char* file) 
		: ebo(0), vbo(0), vao(0)
	{
		if (file) {
			getMeshFromFile(file);
			generateBuffers();
		}
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Render()
	{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, (GLsizei)ebo_data.size(), GL_UNSIGNED_INT, 0);
	}
}