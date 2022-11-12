#pragma once
#include <filesystem>

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
		static void LoadScene();
	private:
		ProjectManager();
	};
}