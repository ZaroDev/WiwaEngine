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

	// SPECIALIZATION FOR SHADER
	template<>
	inline void Resources::LoadMeta<Shader>(const char* file, ModelSettings* settings)
	{

	}
	template<>
	inline void Resources::CreateMeta<Shader>(const char* file)
	{	
		std::string filePath = file;
		filePath += ".meta";
		std::ofstream metaFile(filePath.c_str(), std::ios::out | std::ios::binary);
		metaFile.write(file, filePath.size());
		metaFile.close();
	}
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
	template<>
	inline void Resources::Import<Shader>(const char* file)
	{
		std::string filePath = file;
		filePath += ".vs";
		std::string* vertexShader = getFileData(filePath.c_str());
		
		filePath = file;
		filePath += ".fs";
		std::string* fragmentShader =getFileData(filePath.c_str());
		
		filePath = file;
		filePath += ".gs";
		std::string* geometryShader = getFileData(filePath.c_str());

		std::string shaderFile;

		if (vertexShader)
		{
			shaderFile += "SV\n";
			shaderFile += vertexShader->c_str();
			shaderFile += "\nEV\n";
		}
		if (fragmentShader)
		{
			shaderFile += "SF\n";
			shaderFile += fragmentShader->c_str();
			shaderFile += "\nEF\n";
		}
		if (geometryShader)
		{
			shaderFile += "SG\n";
			shaderFile += geometryShader->c_str();
			shaderFile += "\nEG\n";
		}


		delete vertexShader;
		delete fragmentShader;
		delete geometryShader;

		if (shaderFile.empty())
			return;
		SaveFile(file, shaderFile);
		CreateMeta<Shader>(file, NULL);
	}
	//--SPECIALIZATION FOR SPRITE
	template<>
	inline void Resources::LoadMeta<Image>(const char* file, ImageSettings* settings)
	{
		std::filesystem::path filePath = file;
		filePath += ".meta";
		if (!std::filesystem::exists(filePath))
			return;
		JSONDocument doc(filePath.string().c_str());
		if (!doc.HasMember("imageImportSettings"))
			return;
		settings->Compression = (CompressionType)doc["imageImportSettings"]["compression"].get<int>();
		settings->Interlaced = doc["imageImportSettings"]["interlaced"].get<bool>();
		settings->OffsetX = doc["imageImportSettings"]["offset_x"].get<int>();
		settings->OffsetY = doc["imageImportSettings"]["offset_y"].get<int>();
		settings->Anystropy = doc["imageImportSettings"]["anystropy"].get<bool>();
		settings->MaxAnystropy = doc["imageImportSettings"]["max_anystropy"].get<bool>();
		settings->BlurGausian = doc["imageImportSettings"]["blur_gausian"].get<bool>();
		settings->BlurAverage = doc["imageImportSettings"]["blur_average"].get<bool>();
		settings->Contrast = doc["imageImportSettings"]["contrast"].get<bool>();
		settings->AmountOfContrast = doc["imageImportSettings"]["amount_of_contrast"].get<int>();
		settings->Alienify = doc["imageImportSettings"]["alienify"].get<bool>();
		settings->GammaCorrection = doc["imageImportSettings"]["gamma_correction"].get<bool>();
		settings->Noise = doc["imageImportSettings"]["noise"].get<bool>();
		settings->Equialize = doc["imageImportSettings"]["equialize"].get<bool>();
		settings->Pixelize = doc["imageImportSettings"]["pixelize"].get<bool>();
		settings->Sharpen = doc["imageImportSettings"]["sharpen"].get<bool>();
		settings->SharpenFactor = doc["imageImportSettings"]["sharpen_factor"].get<int>();
		settings->SharpenIterations = doc["imageImportSettings"]["sharpen_iterations"].get<int>();
		settings->Scale = doc["imageImportSettings"]["scale"].get<bool>();
		doc.save_file(filePath.string().c_str());
	}
	template<>
	inline void Resources::CreateMeta<Image>(const char* file, ImageSettings* settings)
	{
		if (!settings)
			return;
		std::filesystem::path filePath = file;
		filePath += ".meta";
		JSONDocument doc;
		doc.AddMember("fileFormatVersion", 1);
		doc.AddMember("file", file);
		doc.AddMember("folderAsset", false);
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		doc.AddMember("timeCreated", std::ctime(&time));
		JSONValue imageSettingsObj = doc.AddMemberObject("imageImportSettings");
		imageSettingsObj.AddMember("compression", (int)settings->Compression);
		imageSettingsObj.AddMember("interlaced", settings->Interlaced);
		imageSettingsObj.AddMember("offset_x", (int)settings->OffsetX);
		imageSettingsObj.AddMember("offset_y", (int)settings->OffsetY);
		imageSettingsObj.AddMember("anystropy", settings->Anystropy);
		imageSettingsObj.AddMember("max_anystropy", settings->MaxAnystropy);
		imageSettingsObj.AddMember("blur_gausian", settings->BlurGausian);
		imageSettingsObj.AddMember("blur_average", settings->BlurAverage);
		imageSettingsObj.AddMember("contrast", settings->Contrast);
		imageSettingsObj.AddMember("amount_of_contrast", (int)settings->AmountOfContrast);
		imageSettingsObj.AddMember("alienify", settings->Alienify);
		imageSettingsObj.AddMember("gamma_correction", settings->GammaCorrection);
		imageSettingsObj.AddMember("noise", settings->Noise);
		imageSettingsObj.AddMember("equialize", settings->Equialize);
		imageSettingsObj.AddMember("pixelize", settings->Pixelize);
		imageSettingsObj.AddMember("sharpen", settings->Sharpen);
		imageSettingsObj.AddMember("sharpen_factor", (int)settings->SharpenFactor);
		imageSettingsObj.AddMember("sharpen_iterations", (int)settings->SharpenIterations);
		imageSettingsObj.AddMember("scale", settings->Scale);
		doc.save_file(filePath.string().c_str());
	}
	template<>
	inline ResourceId Resources::Load<Image>(const char * file)
	{
		ResourceId position = getResourcePosition(WRT_IMAGE, file);
		size_t size = m_Resources[WRT_IMAGE].size();
		ImageSettings* settings = new ImageSettings();
		//CreateMeta<Image>(file, settings);
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

	template<>
	inline void Resources::Import<Image>(const char* file)
	{
		std::filesystem::path load_path = file;

		std::filesystem::path import_path = "library/";
		import_path += load_path.filename();
		import_path.replace_extension(".dds");

		_import_image_impl(file, import_path.string().c_str());
	}

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
		doc.AddMember("timeCreated", std::ctime(&time));
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

			PushResource(WRT_MODEL, file, model);

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
	inline Wiwa::Material* Resources::GetResourceById<Wiwa::Material>(ResourceId id)
	{
		Wiwa::Material* material = NULL;

		if (id >= 0 && id < m_Resources[WRT_MATERIAL].size()) {
			material = static_cast<Wiwa::Material*>(m_Resources[WRT_MATERIAL][id]->resource);
		}

		return material;
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
