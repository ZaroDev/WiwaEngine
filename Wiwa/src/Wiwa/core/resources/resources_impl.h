#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/core/Core.h>

// Resources
#include <Wiwa/utilities/render/Shader.h>
#include <Wiwa/utilities/render/Image.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/json/JSONDocument.h>
#include <Wiwa/utilities/json/JSONValue.h>

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>

typedef size_t ResourceId;

namespace Wiwa {
	enum class CompressionType {
		NONE = 0,
		RGB_DXT1,
		RGBA_DXT1,
		RGBA_DXT3,
		RGBA_DXT5
	};
	struct ImageSettings {
		CompressionType Compression = CompressionType::RGBA_DXT5;
		bool Interlaced = false;
		int32_t OffsetX = 0;
		int32_t OffsetY = 0;
		bool MipMapping = true;
		bool Anystropy = true;
		bool MaxAnystropy = true;
		bool BlurGausian = false;
		bool BlurAverage = false;
		bool Contrast = false;
		int32_t AmountOfContrast = 0;
		bool Alienify = false;
		bool GammaCorrection = false;
		bool Noise = false;
		bool Equialize = false;
		bool Negative = false;
		bool Pixelize = false;
		bool Sharpen = false;
		int32_t SharpenFactor = 0;
		int32_t SharpenIterations = 0;
		bool Scale = 0;
		ImageSettings() = default;
	};

	struct ModelSettings {
		bool preTranslatedVertices = false;
		ModelSettings() = default;
	};
	class WI_API Resources
	{
	public:
		// Enum that indicates all the resources that can be loaded by the engine
		enum ResourceType {
			WRT_IMAGE,
			WRT_AUDIOCLIP, //TODO
			WRT_SHADER,
			WRT_MODEL,
			WRT_MATERIAL,
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

		// Resource path for importing
		static std::string _assetToLibPath(std::string path);
		static bool _preparePath(std::string path);

		// Implementations
		static void _import_image_impl(const char* origin, const char* destination);
		static void _import_model_impl(const char* origin, const char* destination, ModelSettings* settings);
	public:
		static void _toLower(std::string& path);
		inline static std::vector<Resource*> GetResourcesOf(ResourceType rt) { return m_Resources[rt]; }
		template<class T> static ResourceId Load(const char* file);
		template<class T> static ResourceId LoadNative(const char* file);
		template<class T> static T* GetResourceById(ResourceId id);
		template<class T, class... T2> static void Import(const char* file, T2... settings);
		template<class T, class... T2> static void CreateMeta(const char* file, T2... settings);
		template<class T, class... T2> static void LoadMeta(const char* file, T2... settings);

		inline static void SaveFile(const char* file, std::string& shaderFile)
		{
			std::string path = "Library/";
			path += file;
			size_t index = path.rfind('/');
			path = path.substr(0, index);
			if (!std::filesystem::exists(path))
			{
				if (std::filesystem::create_directories(path))
					WI_CORE_ERROR("Can't create a folder at {0}", path.c_str());
			}
			path = "Library/";
			path += file;
			path += ".wiasset";
			std::ofstream outFile(path.c_str(), std::ios::out | std::ios::binary);
			outFile.write(shaderFile.c_str(), shaderFile.size());
			outFile.close();
		}
		inline static std::string* getFileData(const char* file)
		{
			std::fstream shaderFile;

			shaderFile.open(file, std::ios::in);

			if (!shaderFile.is_open()) return NULL;

			std::stringstream buffer;

			buffer << shaderFile.rdbuf();

			shaderFile.close();

			return new std::string(buffer.str());
		}
		static void Clear();
	};
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
