#include <wipch.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {
	std::vector<Resources::Resource*> Resources::m_Resources[Resources::WRT_LAST];

	Resources::Resources()
	{
	}

	void Resources::PushResource(ResourceType rt, const char* file, void* rsc)
	{
		Resource* resource = new Resource();

		resource->filePath = file;
		resource->resource = rsc;

		m_Resources[rt].push_back(resource);

		std::string message = "Loaded resource \"" + resource->filePath + "\" successfully.";

		WI_CORE_INFO(message.c_str());
	}

	ResourceId Resources::getResourcePosition(ResourceType rt, const char * file)
	{
		size_t size = m_Resources[rt].size();

		ResourceId resourceId = size;

		for (size_t i = 0; i < size; i++) {
			if (m_Resources[rt][i]->filePath == file) {
				resourceId = i;
				break;
			}
		}

		return resourceId;
	}

	void Resources::Clear()
	{
		/*for (int i = 0; i < RT_LAST; i++) {
			size_t rsize = mResources[i].size();

			for (size_t j = 0; j < rsize; j++) {
				delete mResources[i][j]->resource;
				delete mResources[i][j];
			}
		}*/
	}
}