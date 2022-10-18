#pragma once

#include <Wiwa/Layer.h>
#include <Wiwa/utilities/containers/List.h>

#include "Panels/Panel.h"

#include "Panels/AboutPanel.h"
#include "Panels/ConfigurationPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/ScenePanel.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/AssetsPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/MeshViewPanel.h"

typedef void* ImTextureID;

class EditorLayer : public Wiwa::Layer
{
public:
	EditorLayer();
	~EditorLayer();
	

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Wiwa::Event& e) override;
private:
	void MainMenuBar();
	void DockSpace();

	void LoadPanelConfig();
	void SavePanelConfig();

	bool OnKeyPressed(Wiwa::KeyPressedEvent& e);

private:
	bool m_ShowConsole = false;
	bool m_ShowDemo = false;

	std::shared_ptr<AboutPanel>			m_About = nullptr;
	std::shared_ptr<ConfigurationPanel> m_Configuration = nullptr;
	std::shared_ptr<ConsolePanel>		m_Console = nullptr;
	std::shared_ptr<ScenePanel>			m_Scene = nullptr;
	std::shared_ptr<HierarchyPanel>		m_Hierarchy = nullptr;
	std::shared_ptr<AssetsPanel>		m_Assets = nullptr;
	std::shared_ptr<InspectorPanel>		m_Inspector = nullptr;
	std::shared_ptr<MeshViewPanel>		m_MeshView = nullptr;

	std::vector<std::shared_ptr<Panel>> m_Panels;

	ImTextureID m_PlayIcon;
	ImTextureID m_PauseIcon;
	ImTextureID m_InfoIcon;
	ImTextureID m_WarningIcon;
	ImTextureID m_ErrorIcon;

	int m_GizmoType = -1;
};