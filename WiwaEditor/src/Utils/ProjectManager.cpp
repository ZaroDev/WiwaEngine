#include <wipch.h>
#include "ProjectManager.h"
#include <Wiwa/utilities/json/JSONDocument.h>

namespace Utils
{
	std::filesystem::path ProjectManager::m_CurrentProject = "";

	void ProjectManager::CreateProject(const char* file)
	{
		Wiwa::JSONDocument doc;
		doc.AddMember("scenes", NULL);
		std::string path = file;
		path += ".wiproject";
		doc.save_file(path.c_str());
		m_CurrentProject = path.c_str();
	}
	void ProjectManager::OpenProject(const char* file)
	{
		Wiwa::JSONDocument doc(file);
		m_CurrentProject = file;

	}

	void ProjectManager::SaveProject()
	{
	}

	void ProjectManager::SaveScene(const char* file)
	{
	}

	void ProjectManager::LoadScene()
	{
	}
}