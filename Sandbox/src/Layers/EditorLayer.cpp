#include "EditorLayer.h"

#include <imgui.h>
#include <stdio.h>
#include <Wiwa/Application.h>

EditorLayer::EditorLayer()
	: Layer("Editor Layer")
{

}

EditorLayer::~EditorLayer()
{

}

void EditorLayer::OnAttach()
{
	m_About = new AboutPanel();
}

void EditorLayer::OnDetach()
{
	delete m_About;
}

void EditorLayer::OnUpdate()
{

}

void EditorLayer::OnImGuiRender()
{
	ImGuiContext* ctx = Wiwa::Application::Get().GetImGuiContext();
	ImGui::SetCurrentContext(ctx);

	MainMenuBar();

	if (m_About->active)
		m_About->Draw();

}




void EditorLayer::OnEvent(Wiwa::Event& event)
{
}

void EditorLayer::MainMenuBar()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit"))
		{
			Wiwa::Application::Get().Quit();
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Configuration"))
			m_ShowConfig = !m_ShowConfig;
		if (ImGui::MenuItem("Console"))
			m_ShowConfig = !m_ShowConfig;
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("GuiDemo"))
			m_ShowDemo = !m_ShowDemo;
		if (ImGui::MenuItem("Documentation"))
			Wiwa::Application::Get().RequestBrowser("https://github.com/ZaroDev/WiwaEngine/wiki");
		if (ImGui::MenuItem("Download Latest"))
			Wiwa::Application::Get().RequestBrowser("https://github.com/ZaroDev/WiwaEngine/releases");
		if (ImGui::MenuItem("Report a bug"))
			Wiwa::Application::Get().RequestBrowser("https://github.com/ZaroDev/WiwaEngine/issues");
		if (ImGui::MenuItem("About"))
			m_About->SwitchActive();
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	static bool show = true;
	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);
}
