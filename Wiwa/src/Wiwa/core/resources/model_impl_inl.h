#pragma once
#include "resources_impl.h"

namespace Wiwa {
	//--SPECIALIZATION FOR MODEL
	template<>
	inline void Resources::LoadMeta<Model>(const char* file, ModelSettings* settings)
	{
		std::filesystem::path filePath = file;
		filePath += ".meta";
		if (!std::filesystem::exists(filePath))
			return;
		JSONDocument doc(filePath.string().c_str());
		if (!doc.HasMember("modelImportSettings"))
			return;
		settings->preTranslatedVertices = doc["modelImportSettings"]["pre_translated_vertices"].get<bool>();
	}
	template<>
	inline void Resources::CreateMeta<Model>(const char* file, ModelSettings* settings)
	{
		std::filesystem::path filePath = file;
		filePath += ".meta";
		JSONDocument doc;
		doc.AddMember("fileFormatVersion", 1);
		doc.AddMember("file", file);
		doc.AddMember("folderAsset", false);
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char buff[64];
		ctime_s(buff, sizeof(buff), &time);
		doc.AddMember("timeCreated", buff);
		JSONValue modelSettignsObject = doc.AddMemberObject("modelImportSettings");
		modelSettignsObject.AddMember("pre_translated_vertices", settings->preTranslatedVertices);
		doc.save_file(filePath.string().c_str());
	}
	template<>
	inline ResourceId Resources::LoadNative<Model>(const char* file)
	{
		ResourceId position = getResourcePosition(WRT_MODEL, file);
		size_t size = m_Resources[WRT_MODEL].size();

		ResourceId resourceId;

		if (position == size) {
			Model* model = new Model(file);

			PushResource(WRT_MODEL, file, model, true);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline ResourceId Resources::Load<Model>(const char* file)
	{
		ResourceId position = getResourcePosition(WRT_MODEL, file);
		size_t size = m_Resources[WRT_MODEL].size();

		ResourceId resourceId;

		if (position == size) {
			Model* model = new Model(NULL);
			std::string file_path = "library/";
			file_path += file;
			file_path += ".wimodel";
			model->LoadWiMesh(file_path.c_str());

			PushResource(WRT_MODEL, file, model);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline Model* Resources::GetResourceById<Model>(ResourceId id)
	{
		Model* model = NULL;

		if (id >= 0 && id < m_Resources[WRT_MODEL].size()) {
			model = static_cast<Model*>(m_Resources[WRT_MODEL][id]->resource);
		}

		return model;
	}
	template<>
	inline void Resources::Import<Model>(const char* file, ModelSettings* settings)
	{
		std::filesystem::path import_file = file;
		std::filesystem::path export_file = _assetToLibPath(file);
		export_file.replace_extension(".wimodel");

		std::filesystem::path export_path = export_file.parent_path();

		if (_preparePath(export_path.string())) {
			_import_model_impl(import_file.string().c_str(), export_file.string().c_str(), settings);
		}
	}
	template<>
	inline const char* Resources::getResourcePathById<Model>(size_t id)
	{
		return getPathById(WRT_MODEL, id);
	}
}