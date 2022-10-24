#pragma once

#include <Wiwa/Layer.h>
#include <Wiwa/utilities/containers/List.h>

#include "Panels/Panel.h"
#include <Wiwa/utilities/Action.h>
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
	void DockSpace();

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
	std::unique_ptr<Panel>	m_Inspector ;
	std::unique_ptr<Panel>	m_MeshView;
	std::unique_ptr<Panel>	m_MaterialEditor;

	std::vector<Panel*> m_Panels;

	ImTextureID m_PlayIcon = 0;
	ImTextureID m_PauseIcon = 0;
	ImTextureID m_InfoIcon = 0;
	ImTextureID m_WarningIcon = 0;
	ImTextureID m_ErrorIcon = 0;

	Action<Wiwa::Event&> m_EventCallback;

	int m_GizmoType = -1;
};