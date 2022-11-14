#include <wipch.h>
#include "ScriptUtils.h"

#include <mono/metadata/assembly.h>
#include <Wiwa/utilities/filesystem/FileSystem.h>
#include <mono/metadata/class.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	namespace Utils
	{
		char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}
		MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				WI_CORE_ERROR("ScriptEngine {0}", errorMessage);
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;
			WI_CORE_INFO("Succesfully loaded assembly at {0}", assemblyPath.string().c_str());
			return assembly;
		}
		MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			MonoClass* klass = mono_class_from_name(image, namespaceName, className);

			if (klass == nullptr)
			{
				WI_CORE_ERROR("Mono can't get the class {0}.{1} from the assembly image ", namespaceName, className);
				return nullptr;
			}

			return klass;
		}
		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
			File file = FileSystem::OpenO("assembly.txt");
			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				file << nameSpace << "." << name << "\n";

				WI_CORE_INFO("{0}.{1}", nameSpace, name);
			}
			file.Close();
		}
		void PrintReflectionTypes(std::unordered_map<size_t, Type*> types)
		{
			for (auto& i = types.begin(); i != types.end(); i++)
			{
				WI_CORE_INFO("{0}", i->second->name);
			}
		}
	}
}