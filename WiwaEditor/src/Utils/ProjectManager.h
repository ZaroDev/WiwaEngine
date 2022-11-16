#pragma once
#include <filesystem>
#include <vector>
#include <Wiwa/scene/SceneManager.h>

namespace Utils
{
	class ProjectManager
	{
	private:
		static std::filesystem::path m_CurrentProject;


	public:
		static void CreateProject(const char* file);
		static void OpenProject(const char* file);
		static void SaveProject();
		static void SaveProjectAs(const char* file);


		static void SaveScene(const char* file);
		static void LoadScene(const char* file);
	private:
		ProjectManager();
	};
}