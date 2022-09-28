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
	m_Configuration = new ConfigurationPanel();
	m_Console = new ConsolePanel();
}

void EditorLayer::OnDetach()
{
	delete m_About;
	delete m_Configuration;
}

void EditorLayer::OnUpdate()
{
	if(m_Configuration->active)
		m_Configuration->Update();
}

void EditorLayer::OnImGuiRender()
{
	ImGuiContext* ctx = Wiwa::Application::Get().GetImGuiContext();
	ImGui::SetCurrentContext(ctx);

	MainMenuBar();
	DockSpace();

	if (m_About->active)
		m_About->Draw();
	if (m_Configuration->active)
		m_Configuration->Draw();
	if (m_Console->active)
		m_Configuration->Draw();
	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);


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
		if (ImGui::MenuItem("Configuration", "", m_Configuration->active))
			m_Configuration->SwitchActive();

		if (ImGui::MenuItem("Console", "", m_Console->active))
			m_Console->SwitchActive();

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("GuiDemo", "", m_ShowDemo))
			m_ShowDemo = !m_ShowDemo;

		if (ImGui::MenuItem("Documentation"))
			Wiwa::Application::Get().RequestBrowser("https://github.com/ZaroDev/WiwaEngine/wiki");

		if (ImGui::MenuItem("Download Latest"))
			Wiwa::Application::Get().RequestBrowser("https://github.com/ZaroDev/WiwaEngine/releases");

		if (ImGui::MenuItem("Report a bug"))
			Wiwa::Application::Get().RequestBrowser("https://github.com/ZaroDev/WiwaEngine/issues");

		if (ImGui::MenuItem("About", "", m_About->active))
			m_About->SwitchActive();

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	
}

void EditorLayer::DockSpace()
{
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	static bool show = false;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &show, window_flags);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);
	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
}
