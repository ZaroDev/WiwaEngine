#pragma once
#include "resources_impl.h"
namespace Wiwa {
	//--SPECIALIZATION FOR MATERIAL
	template<>
	inline void Resources::LoadMeta<Material>(const char* file)
	{

	}
	template<>
	inline void Resources::CreateMeta<Material>(const char* file)
	{

	}
	template<>
	inline ResourceId Resources::Load<Material>(const char* file)
	{
		std::filesystem::path library_file = _assetToLibPath(file);

		std::string file_path = library_file.string();
		standarizePath(file_path);

		ResourceId position = getResourcePosition(WRT_MATERIAL, file_path.c_str());
		size_t size = m_Resources[WRT_MATERIAL].size();

		ResourceId resourceId;

		if (position == size) {

			Material* material = new Material(file_path.c_str());

			PushResource(WRT_MATERIAL, file_path.c_str(), material);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline Material* Resources::GetResourceById<Material>(ResourceId id)
	{
		Material* material = NULL;

		if (id >= 0 && id < m_Resources[WRT_MATERIAL].size()) {
			material = static_cast<Material*>(m_Resources[WRT_MATERIAL][id]->resource);
		}

		return material;
	}
	template<>
	inline void Resources::Import<Material>(const char* file)
	{
		std::filesystem::path import_file = file;
		std::filesystem::path export_file = _assetToLibPath(file);
		export_file.replace_extension(".wimaterial");

		std::filesystem::path export_path = export_file.parent_path();

		if (_preparePath(export_path.string())) {
			std::ifstream source(file, std::ios::binary);
			std::ofstream dest(export_file.c_str(), std::ios::binary);

			dest << source.rdbuf();

			source.close();
			dest.close();
		}
	}
	template<>
	inline const char* Resources::getResourcePathById<Material>(size_t id)
	{
		return getPathById(WRT_MATERIAL, id);
	}
}