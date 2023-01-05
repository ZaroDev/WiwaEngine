#pragma once

#include <Wiwa/core/Layer.h>
#include <Wiwa/utilities/containers/List.h>

#include "Panels/Panel.h"
#include <Wiwa/utilities/functions/Action.h>
#include <Wiwa/Events/Event.h>

#include "../Events/PanelEvents.h"

#include "Panels/AboutPanel.h"
#include "Panels/ConfigurationPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/ScenePanel.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/AssetsPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/MeshViewPanel.h"
#include "Panels/MaterialPanel.h"
#include "Panels/ProjectPanel.h"
#include "Panels/SystemsPanel.h"
#include "Panels/GamePanel.h"
#include "Panels/CamerasPanel.h"
#include "Panels/ResourcesPanel.h"
#include "Panels/ImportPanel.h"
#include "Panels/ShaderPanel.h"

#include <Wiwa/scene/SceneManager.h>

#include <memory>

typedef void* ImTextureID;

class EditorLayer : public Wiwa::Layer
{
	typedef std::function<void(Wiwa::Event&)> EventCallbackFn;
public:
	EditorLayer();
	~EditorLayer();
	

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Wiwa::Event& e) override;

	inline int GetGizmo() { return m_GizmoType; }
private:
	void MainMenuBar();
	void OpenCloseAssetsFolder();
	void SaveProjectAs();
	void OpenProject();
	void NewProject();
	void DockSpace();

	// Scene functions
	void SaveSceneAs();
	void OpenScene();

	void LoadPanelConfig();
	void SavePanelConfig();

	void LoadCallback();
	void SaveCallback();

	bool OnKeyPressed(Wiwa::KeyPressedEvent& e);
	bool OnLoad(Wiwa::OnLoadEvent& e);
	bool OnSave(Wiwa::OnSaveEvent& e);
	bool OnWindowClose(Wiwa::WindowCloseEvent& e);
private:
	bool m_ShowConsole = false;
	bool m_ShowDemo = false;

	std::unique_ptr<Panel>	m_About;
	std::unique_ptr<Panel>	m_Configuration;
	std::unique_ptr<Panel>	m_Console;
	std::unique_ptr<Panel>	m_Scene;
	std::unique_ptr<Panel>	m_Hierarchy;
	std::unique_ptr<Panel>	m_Assets;
	std::unique_ptr<Panel>	m_Inspector;
	std::unique_ptr<Panel>	m_MeshView;
	std::unique_ptr<Panel>	m_MaterialEditor;
	std::unique_ptr<Panel>	m_ProjectPanel;
	std::unique_ptr<Panel>	m_SystemsPanel;
	std::unique_ptr<Panel>	m_GamePanel;
	std::unique_ptr<Panel>	m_CamerasPanel;
	std::unique_ptr<Panel>	m_ResourcesPanel;
	std::unique_ptr<Panel>	m_ImportPanel;
	std::unique_ptr<Panel>	m_ShaderPanel;

	std::vector<Panel*> m_Panels;
	std::vector<Panel*> m_Settings;

	ImTextureID m_PlayIcon = 0;
	ImTextureID m_PauseIcon = 0;
	ImTextureID m_InfoIcon = 0;
	ImTextureID m_WarningIcon = 0;
	ImTextureID m_ErrorIcon = 0;
	ImTextureID m_StopIcon = 0;

	Action<Wiwa::Event&> m_EventCallback;

	SceneId m_EditorSceneId;
	Wiwa::Scene* m_EditorScene;

	int m_GizmoType = -1;
};