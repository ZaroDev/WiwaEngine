#include "EditorLayer.h"

#include <imgui.h>
#include <stdio.h>

#include <Wiwa/Application.h>
#include <Wiwa/utilities/json/JSONDocument.h>

#include <Wiwa/Input.h>
#include <ImGuizmo.h>

#include <imgui_internal.h>

#include <Wiwa/Resources.h>

EditorLayer::EditorLayer()
	: Layer("Editor Layer")
{

}

EditorLayer::~EditorLayer()
{

}

void EditorLayer::OnAttach()
{
	m_About = std::make_shared<AboutPanel>();
	m_Configuration = std::make_shared<ConfigurationPanel>();
	m_Console = std::make_shared<ConsolePanel>();
	m_Scene = std::make_shared<ScenePanel>();
	m_Hierarchy = std::make_shared<HierarchyPanel>();
	m_Assets = std::make_shared<AssetsPanel>();
	m_Inspector = std::make_shared<InspectorPanel>();
	m_MeshView = std::make_shared<MeshViewPanel>();

	m_Panels.push_back(m_Configuration);
	m_Panels.push_back(m_Console);
	m_Panels.push_back(m_Scene);
	m_Panels.push_back(m_Hierarchy);
	m_Panels.push_back(m_Assets);
	m_Panels.push_back(m_Inspector);
	m_Panels.push_back(m_MeshView);

	LoadPanelConfig();

	ResourceId playId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/play_icon.png");
	ResourceId pauseId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/pause_icon.png");

	m_PlayIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(playId)->GetTextureId();
	m_PauseIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(pauseId)->GetTextureId();

	WI_TRACE("Editor layer attached!");
}

void EditorLayer::OnDetach()
{
	SavePanelConfig();
	m_Panels.clear();
}

void EditorLayer::OnUpdate()
{
	if(m_Configuration->active)
		m_Configuration->Update();
	if (m_Assets->active)
		m_Assets->Update();
}

void EditorLayer::OnImGuiRender()
{
	ImGuiContext* ctx = Wiwa::Application::Get().GetImGuiContext();
	ImGui::SetCurrentContext(ctx);

	MainMenuBar();
	DockSpace();

	for (auto& p : m_Panels)
	{
		if (p->active)
			p->Draw();
	}
	if (m_About->active)
		m_About->Draw();
	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);


}

void EditorLayer::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Wiwa::KeyPressedEvent>(WI_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
}

void EditorLayer::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close", "ALT + Q"))
				Wiwa::Application::Get().Quit();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			int i = 1;
			for (auto& p : m_Panels)
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
	ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	float height = ImGui::GetFrameHeight();

	if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
		if (ImGui::BeginMenuBar()) 
		{
			if(ImGui::Button("All"))
				m_Scene->SetGizmoType(-1);

			if (ImGui::Button("Trns"))
				m_Scene->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);

			if (ImGui::Button("Rot"))
				m_Scene->SetGizmoType(ImGuizmo::OPERATION::ROTATE);

			if (ImGui::Button("Scl"))
				m_Scene->SetGizmoType(ImGuizmo::OPERATION::SCALE);

			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			ImGui::SetCursorPosX(Wiwa::Application::Get().GetWindow().GetWidth() / 2 - 15.0f);
			ImGui::ImageButton(m_PlayIcon, { 15, 15 });
			ImGui::ImageButton(m_PauseIcon, { 15, 15 });
			ImGui::PopStyleColor();
			
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	if (ImGui::BeginViewportSideBar("##MainStatusBar", viewport, ImGuiDir_Down, height, window_flags)) {
		if (ImGui::BeginMenuBar()) {
			
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
	
}

void EditorLayer::DockSpace()
{
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground;

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
		const auto& p = m_Panels[i];

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
		const auto& p = m_Panels[i];

		config.AddMember(p->GetName(), p->active);
	}

	config.save_file("config/panels.json");
}

bool EditorLayer::OnKeyPressed(Wiwa::KeyPressedEvent& e)
{
	// Shortcuts
	if (e.IsRepeat())
		return false;

	bool control = Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftControl) || Wiwa::Input::IsKeyPressed(Wiwa::Key::RightControl);
	bool shift = Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftShift) || Wiwa::Input::IsKeyPressed(Wiwa::Key::RightShift);
	bool alt = Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftAlt) || Wiwa::Input::IsKeyPressed(Wiwa::Key::RightAlt);

	switch (e.GetKeyCode())
	{
	case Wiwa::Key::N:
	{
		if (control)
			//NewScene();

		break;
	}
	case Wiwa::Key::O:
	{
		if (control)
			//OpenScene();

		break;
	}
	case Wiwa::Key::S:
	{
		if (control)
		{
			//if (shift)
			//	//SaveSceneAs();
			//else
			//	//SaveScene();
		}

		break;
	}

	// Scene Commands
	case Wiwa::Key::D:
	{
		if (control)
			//OnDuplicateEntity();

		break;
	}

	// Gizmos
	case Wiwa::Key::Q:
	{
		if (alt)
			Wiwa::Application::Get().Quit();
		if (!ImGuizmo::IsUsing())
		{
			m_Scene->SetGizmoType(-1); 
			m_GizmoType = -1;
		}
		break;
	}
	case Wiwa::Key::W:
	{
		if (!ImGuizmo::IsUsing())
		{
			m_Scene->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		break;
	}
	case Wiwa::Key::E:
	{
		if (!ImGuizmo::IsUsing())
		{
			m_Scene->SetGizmoType(ImGuizmo::OPERATION::ROTATE);
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		}
		break;
	}
	case Wiwa::Key::R:
	{
		if (!ImGuizmo::IsUsing())
		{
			m_Scene->SetGizmoType(ImGuizmo::OPERATION::SCALE); 
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
		break;
	}
	}
}
