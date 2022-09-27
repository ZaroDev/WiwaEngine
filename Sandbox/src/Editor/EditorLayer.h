#pragma once

#include <Wiwa/Layer.h>

#include <Wiwa/utilities/containers/List.h>

class EditorLayer : public Wiwa::Layer
{
public:
	EditorLayer();
	~EditorLayer();
	

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void FrameHistogram();
	void OnEvent(Wiwa::Event& event) override;
private:
	void MainMenuBar();
private:
	Wiwa::List<float> m_FPSLog;
	Wiwa::List<float> m_MSLog;
	bool m_ShowDemo = false;
};