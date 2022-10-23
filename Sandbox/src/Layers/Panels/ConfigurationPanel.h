#pragma once

#include "Panel.h"

#include "Wiwa/utilities/containers/List.h"
#include "Wiwa/Application.h"

class ConfigurationPanel : public Panel
{
public:
	ConfigurationPanel(EditorLayer* instance);
	virtual ~ConfigurationPanel();

	void Draw() override;

	void Update() override;
private:
	Wiwa::List<float> m_FPSLog;
	Wiwa::List<float> m_MSLog;
	Wiwa::List<float> m_MemLog;

	bool m_Fullscreen = false;
	bool m_Resizable = false;
	bool m_VSync = false;

	Wiwa::SysInfo info;
};