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
#include "Panels/MaterialPanel.h"

#include <Wiwa/utilities/Action.h>

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

	AboutPanel*			m_About = nullptr;
	ConfigurationPanel* m_Configuration = nullptr;
	ConsolePanel*		m_Console = nullptr;
	ScenePanel*			m_Scene = nullptr;
	HierarchyPanel*		m_Hierarchy = nullptr;
	AssetsPanel*		m_Assets = nullptr;
	InspectorPanel*		m_Inspector = nullptr;
	MeshViewPanel*		m_MeshView = nullptr;
	MaterialPanel*		m_MaterialEditor = nullptr;

	std::vector<Panel*> m_Panels;

	ImTextureID m_PlayIcon;
	ImTextureID m_PauseIcon;
	ImTextureID m_InfoIcon;
	ImTextureID m_WarningIcon;
	ImTextureID m_ErrorIcon;

	Action<Wiwa::Event&> m_EventCallback;

	int m_GizmoType = -1;
};