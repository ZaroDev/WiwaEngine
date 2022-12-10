#pragma once
#include "resources_impl.h"

namespace Wiwa {
	// SPECIALIZATION FOR SHADER
	template<>
	inline void Resources::LoadMeta<Shader>(const char* file, ModelSettings* settings)
	{

	}
	template<>
	inline void Resources::CreateMeta<Shader>(const char* file)
	{
		std::string filePath = file;
		filePath += ".meta";
		std::ofstream metaFile(filePath.c_str(), std::ios::out | std::ios::binary);
		metaFile.write(file, filePath.size());
		metaFile.close();
	}
	template<>
	inline ResourceId Resources::Load<Shader>(const char* file) {
		ResourceId position = getResourcePosition(WRT_SHADER, file);
		size_t size = m_Resources[WRT_SHADER].size();

		ResourceId resourceId;

		if (position == size) {
			Shader* shader = new Shader();
			shader->Init(file);

			PushResource(WRT_SHADER, file, shader);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline Shader* Resources::GetResourceById<Shader>(ResourceId id) {
		Shader* resource = NULL;

		if (id >= 0 && id < m_Resources[WRT_SHADER].size()) {
			resource = static_cast<Shader*>(m_Resources[WRT_SHADER][id]->resource);
		}

		return resource;
	}
	template<>
	inline void Resources::Import<Shader>(const char* file)
	{
		std::string filePath = file;
		filePath += ".vs";
		std::string* vertexShader = getFileData(filePath.c_str());

		filePath = file;
		filePath += ".fs";
		std::string* fragmentShader = getFileData(filePath.c_str());

		filePath = file;
		filePath += ".gs";
		std::string* geometryShader = getFileData(filePath.c_str());

		std::string shaderFile;

		if (vertexShader)
		{
			shaderFile += "SV\n";
			shaderFile += vertexShader->c_str();
			shaderFile += "\nEV\n";
		}
		if (fragmentShader)
		{
			shaderFile += "SF\n";
			shaderFile += fragmentShader->c_str();
			shaderFile += "\nEF\n";
		}
		if (geometryShader)
		{
			shaderFile += "SG\n";
			shaderFile += geometryShader->c_str();
			shaderFile += "\nEG\n";
		}


		delete vertexShader;
		delete fragmentShader;
		delete geometryShader;

		if (shaderFile.empty())
			return;
		SaveFile(file, shaderFile);
		CreateMeta<Shader>(file, NULL);
	}
}