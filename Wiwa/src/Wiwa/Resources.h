#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/Core.h>

// Resources
#include <Wiwa/utilities/render/Shader.h>
//#include "utilities/Image.h"

#include <string>
#include <vector>

typedef size_t ResourceId;

namespace Wiwa {
	class WI_API Resources
	{
	public:
		// Enum that indicates all the resources that can be loaded by the engine
		enum ResourceType {
			WRT_SPRITE,
			WRT_AUDIOCLIP,
			WRT_SHADER,
			WRT_LAST
		};

		struct Resource {
			// Path to resource
			std::string filePath;

			// Any type
			void* resource;
		};
	private:
		Resources();

		static std::vector<Resource*> m_Resources[WRT_LAST];

		static void PushResource(ResourceType rt, const char* file, void* rsc);
		static ResourceId getResourcePosition(ResourceType rt, const char* file);
	public:
		template<class T> static ResourceId Load(const char* file);
		template<class T> static T* GetResourceById(ResourceId id);

		static void Clear();
	};

	// SPECIALIZATION FOR SHADER
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

	//--SPECIALIZATION FOR SPRITE
	/*template<>
	inline uint32 Resources::Load<Image>(const char * file)
	{
		size_t size = mResources[ResourceType::RT_SPRITE].size();
		size_t i;

		Uint32 resourceId = -1;

		for (i = 0; i < size; i++) {
			if (mResources[RT_SPRITE][i]->filePath == file) {
				resourceId = static_cast<Uint32>(i);
				break;
			}
		}

		if (i == size) {
			Image* sprite = new Image();
			if (sprite->Init(file)) {
				Resource* resource = new Resource();

				resource->filePath = file;

				resource->resource = sprite;

				resourceId = static_cast<uint32>(size);

				mResources[RT_SPRITE].push_back(resource);

				std::string message = "Loaded resource " + resource->filePath + " successfully.";
				Debug::Log("Resources", message.c_str());
			}
			else {
				Debug::LogError("Resources", SDL_GetError());

				delete sprite;
			}
		}

		return resourceId;
	}

	template<>
	inline Image* Resources::GetResourceById<Image>(ResourceId id)
	{
		Image* surface = NULL;
	
		if (id >= 0 && id < mResources[RT_SPRITE].size()) {
			surface = static_cast<Image*>(mResources[RT_SPRITE][id]->resource);
		}
	
		return surface;
	}*/
}

// SPECIALIZATION FOR MIX_CHUNK
//template<>
//inline ResourceId Resources::Load<AudioClip>(const char * file)
//{
//	Uint32 size = static_cast<Uint32>(mResources[ResourceType::RT_AUDIOCLIP].size());
//	Uint32 i;
//
//	Uint32 resourceId = -1;
//
//	for (i = 0; i < size; i++) {
//		if (mResources[RT_AUDIOCLIP][i]->filePath == file) {
//			resourceId = static_cast<Uint32>(i);
//			break;
//		}
//	}
//
//	if (i == size) {
//		Mix_Chunk* audioclip = Mix_LoadWAV(file);
//
//		if (!audioclip) {
//			std::cout << Mix_GetError() << std::endl;
//		}
//		else {
//			Resource* resource = new Resource();
//
//			resource->resource = audioclip;
//
//			resource->filePath = file;
//
//			resourceId = static_cast<Uint32>(size);
//
//			mResources[RT_AUDIOCLIP].push_back(resource);
//
//			std::cout << "RESOURCES: Loaded " << file << std::endl;
//		}
//	}
//
//	return resourceId;
//}

//template<>
//inline AudioClip* Resources::GetResourceById<AudioClip>(ResourceId id)
//{
//	AudioClip* audioclip = NULL;
//
//	if (id >= 0 && id < mResources[RT_AUDIOCLIP].size()) {
//		audioclip = static_cast<AudioClip*>(mResources[RT_AUDIOCLIP][id]->resource);
//	}
//
//	return audioclip;
//}

// BOMBERMAN

// SPECIALIZATION FOR MAP

//template<>
//inline ResourceId Resources::Load<Map>(const char* file) {
//	Uint32 size = static_cast<Uint32>(mResources[ResourceType::RT_MAP].size());
//	Uint32 i;
//
//	Uint32 resourceId = -1;
//
//	for (i = 0; i < size; i++) {
//		if (mResources[RT_MAP][i]->filePath == file) {
//			resourceId = static_cast<Uint32>(i);
//			break;
//		}
//	}
//
//	if (i == size) {
//		Map* map = new Map();
//		map->LoadMap(file);
//
//		Resource* resource = new Resource();
//
//		resource->resource = map;
//
//		resource->filePath = file;
//
//		resourceId = static_cast<Uint32>(size);
//
//		mResources[RT_MAP].push_back(resource);
//
//		std::cout << "RESOURCES: Loaded " << file << std::endl;
//	}
//
//	return resourceId;
//}
//
//template<>
//inline Map* Resources::GetResourceById<Map>(ResourceId resource) {
//	Map* map = NULL;
//
//	if (resource >= 0 && resource < mResources[RT_MAP].size()) {
//		map = static_cast<Map*>(mResources[RT_MAP][resource]->resource);
//	}
//
//	return map;
//}
