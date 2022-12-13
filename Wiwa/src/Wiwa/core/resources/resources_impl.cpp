#include <wipch.h>
#include "resources_impl.h"

#include "../vendor/stb/stb_image.h"

namespace Wiwa {
	std::vector<Resources::Resource*> Resources::m_Resources[Resources::WRT_LAST];

	Resources::Resources()
	{
	}

	void Resources::PushResource(ResourceType rt, const char* file, void* rsc, bool isNative)
	{
		Resource* resource = new Resource();
		resource->isNative = isNative;
		resource->filePath = file;
		resource->resource = rsc;

		m_Resources[rt].push_back(resource);

		std::string message = "Loaded resource \"" + resource->filePath + "\" successfully.";

		WI_CORE_INFO(message.c_str());
	}

	ResourceId Resources::getResourcePosition(ResourceType rt, const char* file)
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

	std::string Resources::_assetToLibPath(std::string path)
	{
		_toLower(path);

		size_t a_ind = path.find("assets");

		if (a_ind != path.npos) {
			path.replace(a_ind, 6, "library");
		}

		return path;
	}

	bool Resources::_preparePath(std::string path)
	{
		if (!std::filesystem::exists(path)) {
			if (!std::filesystem::create_directories(path)) {
				WI_ERROR("Couldn't create directory {}", path.c_str());
				return false;
			}
		}

		return true;
	}

	void Resources::_toLower(std::string& path)
	{
		size_t len = path.size();

		for (size_t i = 0; i < len; i++) {
			if (path[i] >= 'A' && path[i] <= 'Z') path[i] -= ('Z' - 'z');
		}
	}

	void Resources::UnloadSceneResources()
	{
		for (size_t i = 0; i < WRT_LAST; i++)
		{
			for (size_t j = 0; j < m_Resources[i].size(); j++)
			{
				if (!m_Resources[i][j]->isNative)
					m_Resources[i].erase(m_Resources[i].begin() + j);
			}
		}
	}

	void Resources::_import_image_impl(const char* origin, const char* destination)
	{
		int w, h, ch;

		// STBI_rgb_alpha loads image as 32 bpp (4 channels), ch = image origin channels
		unsigned char* image = stbi_load(origin, &w, &h, &ch, STBI_rgb_alpha);
		if (w != h)
		{
			WI_ERROR("Image at {0} needs to be square in order to be imported", origin);
			return;
		}
		Image::raw_to_dds_file(destination, image, w, h, 32);

		stbi_image_free(image);
	}

	void Resources::_import_model_impl(const char* origin, const char* destination, ModelSettings* settings)
	{
		Model* model = Model::GetModelFromFile(origin, settings);

		Model::SaveModel(model, destination);

		delete model;
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