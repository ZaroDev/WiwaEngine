#include <wipch.h>
#include "ProjectManager.h"
#include <Wiwa/utilities/json/JSONDocument.h>
#include <Wiwa/core/Application.h>

namespace Utils
{
	std::filesystem::path ProjectManager::m_CurrentProject = "";

	void ProjectManager::CreateProject(const char* file)
	{
		Wiwa::JSONDocument doc;
		doc.AddMember("name", Wiwa::Application::Get().GetProjectName());
		doc.AddMember("version", Wiwa::Application::Get().GetProjectVersion());
		doc.AddMember("company", Wiwa::Application::Get().GetProjectCompany());
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
		if (doc.HasMember("name"))
			Wiwa::Application::Get().SetProjectName(doc["name"].get<const char*>());
		if (doc.HasMember("version"))
			Wiwa::Application::Get().SetProjectVersion(doc["version"].get<const char*>());
		if (doc.HasMember("company"))
			Wiwa::Application::Get().SetProjectCompany(doc["company"].get<const char*>());

		doc.save_file(file);
	}

	void ProjectManager::SaveProject()
	{
		if (m_CurrentProject.empty())
		{
			return;
		}
		Wiwa::JSONDocument doc(m_CurrentProject.string().c_str());
		doc.AddMember("name", Wiwa::Application::Get().GetProjectName());
		doc.AddMember("version", Wiwa::Application::Get().GetProjectVersion());
		doc.AddMember("company", Wiwa::Application::Get().GetProjectCompany());
		doc.save_file(m_CurrentProject.string().c_str());
	}

	void ProjectManager::SaveProjectAs(const char* file)
	{
		std::string path = file;
		path += ".wiproject";
		Wiwa::JSONDocument doc(path.c_str());
		m_CurrentProject = path;
		doc.AddMember("name", Wiwa::Application::Get().GetProjectName());
		doc.AddMember("version", Wiwa::Application::Get().GetProjectVersion());
		doc.AddMember("company", Wiwa::Application::Get().GetProjectCompany());
		doc.save_file(path.c_str());
	}

	void ProjectManager::SaveScene(const char* file)
	{
	}

	void ProjectManager::LoadScene()
	{
	}
}