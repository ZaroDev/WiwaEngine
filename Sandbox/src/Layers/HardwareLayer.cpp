#include "HardwareLayer.h"

#include "windows.h"
#include "psapi.h"

#include <imgui.h>

HardwareLayer::HardwareLayer()
{
}

HardwareLayer::~HardwareLayer()
{
}

void HardwareLayer::OnAttach()
{
}

void HardwareLayer::OnDetach()
{
}

void HardwareLayer::OnUpdate()
{
	Wiwa::Application::Get().SetHwInfo();

	if (m_MSLog.size() > 64)
		m_MSLog.erase(0);

	if (m_FPSLog.size() > 64)
		m_FPSLog.erase(0);
	if (m_MemLog.size() > 64)
		m_MemLog.erase(0);
}

void HardwareLayer::OnImGuiRender()
{
	info = Wiwa::Application::Get().GetSystemInfo();
	

	HardwareInfo(info);
	HistogramInfo();
}

void HardwareLayer::HardwareInfo(Wiwa::SysInfo& info)
{
	ImGui::Begin("Hardware");
	ImGui::Text("GLFW version: %s", info.glfwVer);
	ImGui::Separator();
	ImGui::Text("CPUs: %i (Cache: %ikb)", info.numCores, info.cache);
	ImGui::Text("System RAM: %0.1fMb", info.ram);
	ImGui::Separator();
	ImGui::Text("GPU: %s", info.gpu);
	ImGui::Text("Brand: %s", info.gpuBrand);
	ImGui::Text("Total VRAM: %0.1fMb", info.gpuVRAM);
	ImGui::Text("VRAM Usage: %0.1fMb", info.gpuVRAMUsage);
	ImGui::Text("VRAM Available: %0.1fMb", info.gpuVRAMAV);
	ImGui::Text("VRAM Reserved: %0.1fMb", info.gpuVRAMReserve);

	ImGui::End();
}

void HardwareLayer::HistogramInfo()
{
	m_MSLog.push_back(1000.0f / ImGui::GetIO().Framerate);
	m_FPSLog.push_back(ImGui::GetIO().Framerate);

	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize >> 20;
	m_MemLog.push_back(virtualMemUsedByMe);

	char title[25];
	ImGui::Begin("Info");
	sprintf_s(title, 25, "Framerate %.1f", m_FPSLog[m_FPSLog.size() - 1]);
	ImGui::PlotHistogram("##framerate", &m_FPSLog[0], m_FPSLog.size(), 0, title, 0.0f, 100.0f, ImVec2(150, 50));
	sprintf_s(title, 25, "Frametime %.1f", m_MSLog[m_MSLog.size() - 1]);
	ImGui::PlotHistogram("##frametime", &m_MSLog[0], m_MSLog.size(), 0, title, 0.0f, 100.0f, ImVec2(150, 50));
	sprintf_s(title, 25, "Mem used %.1f", m_MemLog[m_MemLog.size() - 1]);
	ImGui::PlotHistogram("##memory", &m_MemLog[0], m_MSLog.size(), 0, title, 0.0f, 100.0f, ImVec2(150, 50));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
