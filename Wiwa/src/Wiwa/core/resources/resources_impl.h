#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/core/Core.h>

// Resources
#include <Wiwa/utilities/render/shaders/Shader.h>
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
#include <time.h>

#define RES_NOT_LOADED -1

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
		enum MetaResult
		{
			NOTFOUND,
			DELETED,
			UPDATED,
			TOUPDATE
		};
		struct Resource {
			// Path to resource
			std::string filePath;
			bool isNative;
			// Any type
			void* resource;

			Resource() = default;
		};

	private:
		Resources();

		static std::vector<Resource*> m_Resources[WRT_LAST];

		static void PushResource(ResourceType rt, const char* file, void* rsc, bool isNative = false);
		static ResourceId getResourcePosition(ResourceType rt, const char* file);

		// Resource path for importing
		inline static const char* getPathById(ResourceType type, size_t id)
		{
			if (id >= m_Resources[type].size())
				return "";
			
			return m_Resources[type][id]->filePath.c_str();
		}
		
		
		// Implementations
		static void _import_image_impl(const char* origin, const char* destination);
		static void _import_model_impl(const char* origin, const char* destination, ModelSettings* settings);
		static bool _check_import_impl(const char* file, const char* extension);
		
	public:
		static bool _preparePath(std::string path);
		static std::string _assetToLibPath(std::string path);
		static std::filesystem::path _import_path_impl(const std::filesystem::path& path, const char* extension);
		template <typename TP>
		static inline std::time_t to_time_t(TP tp)
		{
			using namespace std::chrono;
			auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
				+ system_clock::now());
			return system_clock::to_time_t(sctp);
		}
		static inline void standarizePath(std::string& file_path)
		{
			size_t index = 0;

			while ((index = file_path.find('\\', index)) != std::string::npos) {
				file_path.replace(index, 1, "/");
				index++;
			}
			_toLower(file_path);
		}


		static void _toLower(std::string& path);
		inline static std::vector<Resource*>& GetResourcesOf(ResourceType rt) { return m_Resources[rt]; }
		static void UnloadSceneResources();

		// Returns the state of the meta file
		// Deleted: the meta file is deleted due to the reference file not existing
		// Updated: the meta files are up to date
		// ToUpdate: the meta last updated doesnt equal the file reference
		// Not found: there's no meta file for the reference
		static MetaResult CheckMeta(const char* filename);
		static void UpdateMeta(const char* filename);
		template<class T> static const char* getResourcePathById(size_t id);

		template<class T> static ResourceId Load(const char* file);
		template<class T> static ResourceId LoadNative(const char* file);
		template<class T> static T* GetResourceById(ResourceId id);
		template<class T> static bool CheckImport(const char* file);
		template<class T, class... T2> static void Import(const char* file, T2... settings);
		template<class T, class... T2> static void CreateMeta(const char* file, T2... settings);
		template<class T, class... T2> static void LoadMeta(const char* file, T2... settings);

		inline static void SaveFile(const char* file, std::string& shaderFile)
		{
			std::string path = SetLibraryPath(file);

			std::ofstream outFile(path.c_str(), std::ios::out | std::ios::binary);
			outFile.write(shaderFile.c_str(), shaderFile.size());
			outFile.close();
		}

		inline static std::string SetLibraryPath(const char* file)
		{
			std::string path = "library/";
			path += file;
			size_t index = path.rfind('/');
			path = path.substr(0, index);
			if (!std::filesystem::exists(path))
			{
				if (std::filesystem::create_directories(path))
					WI_CORE_ERROR("Can't create a folder at {0}", path.c_str());
			}
			path = "library/";
			path += file;
			path += ".wiasset";

			return path;
		}
		static void SetAssetPath(std::string& path);
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
