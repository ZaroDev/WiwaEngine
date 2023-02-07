#pragma once
#include "resources_impl.h"

namespace Wiwa {
	//--SPECIALIZATION FOR SPRITE
	template<>
	inline void Resources::LoadMeta<Image>(const char* file, ImageSettings* settings)
	{
		std::filesystem::path filePath = file;
		if (!std::filesystem::exists(filePath))
			return;
		filePath += ".meta";
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
		std::time_t time = to_time_t(std::filesystem::last_write_time(file));
		doc.AddMember("timeCreated", time);
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
	inline ResourceId Resources::Load<Image>(const char* file)
	{
		std::filesystem::path file_path = _assetToLibPath(file);
		file_path.replace_extension(".dds");
		ResourceId position = getResourcePosition(WRT_IMAGE, file_path.string().c_str());
		size_t size = m_Resources[WRT_IMAGE].size();
		
		ResourceId resourceId;

		if (position == size) {
			Image* image = new Image();

			image->InitDDS(file_path.string().c_str());

			PushResource(WRT_IMAGE, file_path.string().c_str(), image);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline ResourceId Resources::LoadNative<Image>(const char* file)
	{
		std::string path = file;
		standarizePath(path);
		ResourceId position = getResourcePosition(WRT_IMAGE, path.c_str());
		size_t size = m_Resources[WRT_IMAGE].size();
		ImageSettings* settings = new ImageSettings();
		ResourceId resourceId;

		if (position == size) {
			Image* image = new Image();
			image->Init(path.c_str());

			PushResource(WRT_IMAGE, path.c_str(), image, true);

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
		std::filesystem::path import_path = _import_path_impl(file, ".dds");
		_import_image_impl(file, import_path.string().c_str());

		WI_CORE_INFO("Image at {} imported succesfully!", import_path.string().c_str());
	}
	template<>
	inline bool Resources::CheckImport<Image>(const char* file)
	{
		return _check_import_impl(file, ".dds");
	}
	template<>
	inline const char* Resources::getResourcePathById<Image>(size_t id)
	{
		return getPathById(WRT_IMAGE, id);
	}
}