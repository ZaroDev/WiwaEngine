#pragma once

#include <Wiwa/Layer.h>
#include <Wiwa/utilities/containers/List.h>

#include "Panels/Panel.h"

class EditorLayer : public Wiwa::Layer
{
public:
	EditorLayer();
	~EditorLayer();
	

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Wiwa::Event& event) override;
private:
	void MainMenuBar();
	void DockSpace();
private:
	bool m_ShowConsole = false;
	bool m_ShowDemo = false;

	void LoadPanelConfig();
	void SavePanelConfig();

	std::shared_ptr<Panel> m_About = nullptr;
	std::shared_ptr<Panel> m_Configuration = nullptr;
	std::shared_ptr<Panel> m_Console = nullptr;
	std::shared_ptr<Panel> m_Scene = nullptr;
	std::shared_ptr<Panel> m_Hierarchy = nullptr;
	std::shared_ptr<Panel> m_Assets = nullptr;
	std::shared_ptr<Panel> m_Inspector = nullptr;
	std::shared_ptr<Panel> m_Play = nullptr;

	std::vector<std::shared_ptr<Panel>> m_Panels;
};