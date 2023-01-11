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
		std::filesystem::path filePath = file;
		filePath += ".meta";
		if (!std::filesystem::exists(filePath))
			return;
		JSONDocument doc;
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char buff[64];
		ctime_s(buff, sizeof(buff), &time);
		doc.AddMember("timeCreated", time);
		doc.save_file(filePath.string().c_str());
	}
	template<>
	inline ResourceId Resources::LoadNative<Shader>(const char* file) {
		ResourceId position = getResourcePosition(WRT_SHADER, file);
		size_t size = m_Resources[WRT_SHADER].size();

		ResourceId resourceId;

		if (position == size) {
			Shader* shader = new Shader();

			shader->Init(file);
			//shader->Compile(file);
			PushResource(WRT_SHADER, file, shader);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline ResourceId Resources::Load<Shader>(const char* file) {
		
		std::filesystem::path library_file = _assetToLibPath(file);
		std::string file_path = library_file.string();
		standarizePath(file_path);

		ResourceId position = getResourcePosition(WRT_SHADER, file_path.c_str());
		size_t size = m_Resources[WRT_SHADER].size();

		ResourceId resourceId;

		if (position == size) {
			Shader* shader = new Shader();
			
			shader->LoadFromWiasset(file_path.c_str());
			//shader->Compile(file);
			PushResource(WRT_SHADER, file_path.c_str(), shader);

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
	inline void Resources::Import<Shader>(const char* file, Shader* shader)
	{
		JSONDocument document;
		std::string filePath = file;
		filePath += ".vs";
		std::string* vertexShader = getFileData(filePath.c_str());

		filePath = file;
		filePath += ".fs";
		std::string* fragmentShader = getFileData(filePath.c_str());

		filePath = file;
		filePath += ".gs";
		std::string* geometryShader = getFileData(filePath.c_str());

		if (vertexShader)
			document.AddMember("vertex", vertexShader->c_str());
		if (fragmentShader)
			document.AddMember("fragment", fragmentShader->c_str());
		if (geometryShader)
			document.AddMember("geometry", geometryShader->c_str());
		JSONValue uniformObj = document.AddMemberObject("uniforms");
		if (!shader->getUniforms().empty())
		{
			std::vector<UniformField>& uniforms = shader->getUniforms();
			for(UniformField& uniform : uniforms)
			{
				uniformObj.AddMember(uniform.name.c_str(), (int)uniform.type);
			}
		}

		delete vertexShader;
		delete fragmentShader;
		delete geometryShader;


		std::string file_path = file;
		standarizePath(file_path);
		std::filesystem::path import_file = file_path;
		std::filesystem::path export_file = _assetToLibPath(file_path);
		export_file.replace_extension(".wishader");
		std::filesystem::path export_path = export_file.parent_path();


		if (_preparePath(export_path.string())) {
			document.save_file(export_file.string().c_str());
			CreateMeta<Shader>(file_path.c_str());
		}
	}
	template<>
	inline const char* Resources::getResourcePathById<Shader>(size_t id)
	{
		return getPathById(WRT_SHADER, id);
	}
}