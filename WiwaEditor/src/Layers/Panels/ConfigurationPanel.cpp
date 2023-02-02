#include "ConfigurationPanel.h"

#include <imgui.h>

#include "windows.h"
#include "psapi.h"

#include <Wiwa/utilities/AllocationMetrics.h>
#include "../EditorLayer.h"

ConfigurationPanel::ConfigurationPanel(EditorLayer* instance)
	: Panel("Configuration", ICON_FK_COG, instance), info()
{
	
}

ConfigurationPanel::~ConfigurationPanel()
{
}

void ConfigurationPanel::Draw()
{
	m_Resizable = Wiwa::Application::Get().GetWindow().GetResizable();
	m_Fullscreen = Wiwa::Application::Get().GetWindow().GetFullScreen();
	m_VSync = Wiwa::Application::Get().GetWindow().IsVSync();
	m_MSLog.push_back(Wiwa::Time::GetRealDeltaTime());
	m_FPSLog.push_back(1000.0f / Wiwa::Time::GetRealDeltaTime());
	m_AllocLog.push_back((float)Wiwa::AllocationMetrics::allocation_count);
	//m_ByteLog.push_back((float)Wiwa::AllocationMetrics::bytes_allocated);

	ImGui::Begin(iconName.c_str(), &active);

	if (ImGui::CollapsingHeader("External tools"))
	{
		{		
			const char* types[] = { "vs2022", "vs2019" };
		static const char* currentItem =
			instance->s_SolVersion == "vs2022" ? types[0] : types[1];
		if (ImGui::BeginCombo("Solution version", currentItem))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = (currentItem == types[i]);
				if (ImGui::Selectable(types[i], isSelected))
				{
					currentItem = types[i];
					instance->s_SolVersion = currentItem;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		}
		ImGui::Separator();
		{
			const char* types[] = { "Release", "Debug" };
			static const char* currentItem =
				instance->s_BuildConf == "Release" ? types[0] : types[1];
			if (ImGui::BeginCombo("Build config", currentItem))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = (currentItem == types[i]);
					if (ImGui::Selectable(types[i], isSelected))
					{
						currentItem = types[i];
						instance->s_BuildConf = currentItem;
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (ImGui::Button("Regenerate solution"))
			{
				instance->RegenSol();
			}
		}
	}


	if (ImGui::CollapsingHeader("Window"))
	{
		if (ImGui::Checkbox("Fullscreen", &m_Fullscreen))
			Wiwa::Application::Get().GetWindow().SetFullScreen(m_Fullscreen);
		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &m_Resizable))
			Wiwa::Application::Get().GetWindow().SetResizable(m_Resizable);
		if (ImGui::Checkbox("VSync", &m_VSync))
			Wiwa::Application::Get().GetWindow().SetVSync(m_VSync);
	}
	char title[25];
	if (ImGui::CollapsingHeader("Info"))
	{
		sprintf_s(title, 25, "Framerate %.1f", m_FPSLog[m_FPSLog.size() - 1]);
		ImGui::PlotLines("##framerate", &m_FPSLog[0], (int)m_FPSLog.size(), 0, title, 0.0f, 1000.0f, ImVec2(200, 100));
		sprintf_s(title, 25, "Frametime %.1f", m_MSLog[m_MSLog.size() - 1]);
		ImGui::PlotLines("##frametime", &m_MSLog[0], (int)m_MSLog.size(), 0, title, 0.0f, m_MSLog[0] + 20, ImVec2(200, 100));
		sprintf_s(title, 25, "Mem used %.1f", m_MemLog[m_MemLog.size() - 1]);
		ImGui::PlotHistogram("##memory", &m_MemLog[0], (int)m_MemLog.size(), 0, title, 0.0f, 10000.0f, ImVec2(200, 100));
		sprintf_s(title, 25, "Current Allocations %.0f", m_AllocLog[m_AllocLog.size() - 1]);
		ImGui::PlotHistogram("##memory", &m_AllocLog[0], (int)m_AllocLog.size(), 0, title, 0.0f, 10000.0f, ImVec2(200, 100));
		/*sprintf_s(title, 25, "Bytes allocated %.0f", m_ByteLog[m_ByteLog.size() - 1]);
		ImGui::PlotHistogram("##memory", &m_ByteLog[0], (int)m_ByteLog.size(), 0, title, 0.0f, 90000.0f, ImVec2(200, 100));*/
	
	}
	if (ImGui::CollapsingHeader("Time"))
	{
		ImGui::Text("Time since startup %.2fs", Wiwa::Time::GetRealTimeSinceStartup());
		ImGui::Text("Real time delta time %.3fms", Wiwa::Time::GetRealDeltaTime());
		ImGui::Text("Frame count %i", Wiwa::Time::GetFrameCount());
		if (Wiwa::Time::IsPlaying())
		{
			ImGui::Text("Game time since startup %.2fs", Wiwa::Time::GetTime());
			ImGui::Text("Game delta time %.2fms", Wiwa::Time::GetDeltaTime());
			ImGui::Text("Game frame count %i", Wiwa::Time::GetGameFrameCount());
			float timeScale = Wiwa::Time::GetTimeScale();
			if (ImGui::SliderFloat("Time scale", &timeScale, -2.0f, 5.0f))
			{
				Wiwa::Time::SetTimeScale(timeScale);
			}
		}
		//static ImVec2 curves[64];
		//ImGui::Editor("Chuchawiwa", ImVec2{ 64, 64 }, 64, curves);
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("GLFW version:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", info.glfwVer);
		ImGui::Separator();
		ImGui::Text("CPUs:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", info.numCores);
		ImGui::Text("System RAM:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%0.1fMb", info.ram);
		ImGui::Separator();
		ImGui::Text("GPU:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", info.gpu);
		ImGui::Text("Brand:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", info.gpuBrand);
		ImGui::Text("Total VRAM:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%0.1fMb", info.gpuVRAM);
		ImGui::Text("VRAM Usage:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " %0.1fMb", info.gpuVRAMUsage);
		ImGui::Text("VRAM Available:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%0.1fMb", info.gpuVRAMAV);
		ImGui::Text("VRAM Reserved:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%0.1fMb", info.gpuVRAMReserve);
	}
	ImGui::End();
}

void ConfigurationPanel::Update()
{
	info = Wiwa::Application::Get().GetSystemInfo();
	if (m_MSLog.size() > 64)
		m_MSLog.erase(0);

	if (m_FPSLog.size() > 64)
		m_FPSLog.erase(0);

	if (m_MemLog.size() > 64)
		m_MemLog.erase(0);

	if (m_AllocLog.size() > 64)
		m_AllocLog.erase(0);

	if (m_ByteLog.size() > 64)
		m_ByteLog.erase(0);

	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize >> 20;
	m_MemLog.push_back((float)virtualMemUsedByMe);
}
