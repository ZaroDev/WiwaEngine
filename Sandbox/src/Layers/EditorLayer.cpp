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
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate()
{

}

void EditorLayer::OnImGuiRender()
{
	ImGuiContext* ctx = Wiwa::Application::Get().GetImGuiContext();
	ImGui::SetCurrentContext(ctx);
	if (m_ShowConfig)
	{
		if (ImGui::Begin("Configuration", &m_ShowConfig))
			m_ShowConfig = !m_ShowConfig;
		
		FrameHistogram();

		ImGui::End();
	}
	MainMenuBar();
}


void EditorLayer::FrameHistogram()
{
	

}

void EditorLayer::OnEvent(Wiwa::Event& event)
{
}

void EditorLayer::MainMenuBar()
{
	ImGui::BeginMainMenuBar();
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
		{

		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Windows"))
	{
		if (ImGui::MenuItem("Debug"))
			m_ShowConfig = !m_ShowConfig;

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	static bool show = true;
	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);
}
