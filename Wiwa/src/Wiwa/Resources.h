#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/Core.h>
#include <Wiwa/Application.h>

// Resources
#include <Wiwa/utilities/render/Shader.h>
#include <Wiwa/utilities/render/Image.h>
#include <Wiwa/utilities/render/Model.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

typedef size_t ResourceId;


namespace Wiwa {
	class WI_API Resources
	{
	public:
		// Enum that indicates all the resources that can be loaded by the engine
		enum ResourceType {
			WRT_IMAGE,
			WRT_AUDIOCLIP,
			WRT_SHADER,
			WRT_MODEL,
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
		template<class T> static bool Save(T* resource, const char* file);
		template<class T> static bool Import(T* resource, const char* file);
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
	template<>
	inline ResourceId Resources::Load<Image>(const char * file)
	{
		ResourceId position = getResourcePosition(WRT_IMAGE, file);
		size_t size = m_Resources[WRT_IMAGE].size();

		ResourceId resourceId;

		if (position == size) {
			Image* image = new Image();
			image->Init(file);

			PushResource(WRT_IMAGE, file, image);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}

	template<>
	inline Image* Resources::GetResourceById<Image>(ResourceId id)
	{
		Image* image = NULL;
	
		if (id >= 0 && id < m_Resources[WRT_IMAGE].size()) {
			image = static_cast<Image*>(m_Resources[WRT_IMAGE][id]->resource);
		}
	
		return image;
	}
	//--SPECIALIZATION FOR MODEL
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
	inline bool Resources::Import<Model>(Model* resource, const char* file)
	{
		float time = Application::Get().GetTime();
		std::ifstream infile;
		infile.open(file, std::ios::binary | std::ios::in);
		if (infile.is_open())
		{
			resource->ClearEBO();
			resource->ClearVBO();
			std::string eboString;
			std::string vboString;
	
			std::getline(infile, eboString, ',');
			for (int i = 0; i < eboString.size(); i++)
			{
				resource->AddIndex(eboString[i]);
			}
			std::getline(infile, vboString);
			for (int i = 0; i < vboString.size(); i++)
			{
				resource->AddVertex(vboString[i]);
			}

			float endtime = Application::Get().GetTime();
			endtime -= time;

			WI_CORE_TRACE("Asset imported at {0} in {1} ms", file, endtime);

			return true;
		}

		return false;
	}
	template<>
	inline bool Resources::Save<Model>(Model* resource, const char* file)
	{
		std::ofstream outfile;
		outfile.open(file, std::ios::binary | std::ios::out);
		outfile.write((char*)resource->GetIndicies(), resource->GetNumIndicies());
		outfile.write((char*)resource->GetVerticies(), resource->GetNumVertex());
		outfile.close();

		return true;
	}
	template<>
	inline ResourceId Resources::Load<Model>(const char* file)
	{
		ResourceId position = getResourcePosition(WRT_MODEL, file);
		size_t size = m_Resources[WRT_MODEL].size();

		ResourceId resourceId;

		std::filesystem::path fileString = file;
		std::string filename = fileString.filename().string().c_str();
		std::filesystem::path newPath = "Library/Models";
		filename = filename.substr(0, filename.size() - 4);
		filename += ".wiassets";
		newPath /= filename;

		if (position == size) {

			Model* model = new Model(file);
			//
			//if (!Import<Model>(model, newPath.string().c_str()))
			//{
			//	Save<Model>(model, newPath.string().c_str());
			//}
			//else
			//{
			//	model = new Model(file);
			//}

			PushResource(WRT_MODEL, file, model);
			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
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
