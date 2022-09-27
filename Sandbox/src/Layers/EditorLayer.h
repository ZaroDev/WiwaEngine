#pragma once

#include <Wiwa/Layer.h>
#include <Wiwa/utilities/containers/List.h>

#include "Panels/AboutPanel.h"

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
private:
	bool m_ShowConfig = false;
	bool m_ShowDemo = false;

	AboutPanel* m_About = nullptr;
};