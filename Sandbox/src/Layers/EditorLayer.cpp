#include "EditorLayer.h"

#include <imgui.h>
#include <stdio.h>
#include <Wiwa/Application.h>
#include <Wiwa/utilities/json/JSONDocument.h>

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
	m_Scene = new ScenePanel();
	m_Hierarchy = new HierarchyPanel();
	m_Assets = new AssetsPanel();
	m_Inspector = new InspectorPanel();
	m_Play = new PlayPanel();

	m_Panels.push_back(m_Configuration);
	m_Panels.push_back(m_Console);
	m_Panels.push_back(m_Scene);
	m_Panels.push_back(m_Hierarchy);
	m_Panels.push_back(m_Assets);
	m_Panels.push_back(m_Inspector);

	LoadPanelConfig();

	WI_TRACE("Editor layer attached!");
}

void EditorLayer::OnDetach()
{
	SavePanelConfig();

	delete m_About;
	m_Panels.clear();
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

	for (auto p : m_Panels)
	{
		if (p->active)
			p->Draw();
	}
	//m_Play->Draw();
	if (m_About->active)
		m_About->Draw();
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
		if (ImGui::MenuItem("Close"))
		{
			Wiwa::Application::Get().Quit();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		for (auto p : m_Panels)
		{
			if (ImGui::MenuItem(p->GetName(), "", p->active))
				p->SwitchActive();
		}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("GuiDemo", "", m_ShowDemo))
			m_ShowDemo = !m_ShowDemo;

		if (ImGui::MenuItem("Documentation"))
			Wiwa::Application::Get().OpenDir("https://github.com/ZaroDev/WiwaEngine/wiki");

		if (ImGui::MenuItem("Download Latest"))
			Wiwa::Application::Get().OpenDir("https://github.com/ZaroDev/WiwaEngine/releases");

		if (ImGui::MenuItem("Report a bug"))
			Wiwa::Application::Get().OpenDir("https://github.com/ZaroDev/WiwaEngine/issues");

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

void EditorLayer::LoadPanelConfig()
{
	Wiwa::JSONDocument config("config/panels.json");

	size_t psize = m_Panels.size();

	for (size_t i = 0; i < psize; i++) {
		Panel* p = m_Panels[i];

		if (config.HasMember(p->GetName())) {
			p->active = config[p->GetName()];
		}
	}
}

void EditorLayer::SavePanelConfig()
{
	Wiwa::JSONDocument config;

	size_t psize = m_Panels.size();

	for (size_t i = 0; i < psize; i++) {
		Panel* p = m_Panels[i];

		config.AddMember(p->GetName(), p->active);
	}

	config.save_file("config/panels.json");
}