#pragma once

#include <Wiwa/Layer.h>
#include <Wiwa/Application.h>
#include <Wiwa/utilities/containers/List.h>

class HardwareLayer : public Wiwa::Layer
{
public:
	HardwareLayer();
	~HardwareLayer();


	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void WindowInfo();
private:
	void HardwareInfo(Wiwa::SysInfo& info);
	void HistogramInfo();
private:
	Wiwa::List<float> m_FPSLog;
	Wiwa::List<float> m_MSLog;
	Wiwa::List<float> m_MemLog;

	bool m_Fullscreen = false;
	bool m_Resizable = false;

	Wiwa::SysInfo info;
};