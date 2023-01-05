#pragma once
#include "resources_impl.h"
namespace Wiwa {
	//--SPECIALIZATION FOR MATERIAL
	template<>
	inline void Resources::LoadMeta<Material>(const char* file, ModelSettings* settings)
	{

	}
	template<>
	inline void Resources::CreateMeta<Material>(const char* file, ImageSettings* settings)
	{

	}
	template<>
	inline ResourceId Resources::Load<Material>(const char* file)
	{
		ResourceId position = getResourcePosition(WRT_MATERIAL, file);
		size_t size = m_Resources[WRT_MATERIAL].size();

		ResourceId resourceId;

		if (position == size) {
			Material* material = new Material(file);

			PushResource(WRT_MATERIAL, file, material);

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
	inline const char* Resources::getResourcePathById<Material>(size_t id)
	{
		return m_Resources[WRT_MATERIAL][id]->filePath.c_str();
	}
}