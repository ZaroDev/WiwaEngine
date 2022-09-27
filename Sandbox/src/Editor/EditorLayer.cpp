#include "EditorLayer.h"

#include <imgui.h>
#include <Wiwa/Application.h>
#include <stdio.h>

EditorLayer::EditorLayer()
	: Layer("Editor Layer")
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate()
{
	
}

void EditorLayer::OnImGuiRender()
{
	ImGuiContext* ctx = Wiwa::Application::Get().GetImGuiContext();
	ImGui::SetCurrentContext(ctx);

	Wiwa::SysInfo& info = Wiwa::Application::Get().GetSystemInfo();
	ImGui::Begin("Configuration");
	FrameHistogram();
	ImGui::End();
	ImGui::Begin("Hardware");
	ImGui::Text("GLFW version: %s", info.glfwVer);
	ImGui::Separator();
	ImGui::Text("CPUs: %i (Cache: %ikb)", info.numCores, info.cache);
	ImGui::Text("System RAM: %fGb", info.ram);
	ImGui::Separator();
	ImGui::Text("GPU: %s", info.gpu);
	ImGui::Text("Brand: %s", info.gpuBrand);
	ImGui::Text("Total VRAM: %fGb", info.gpuVRAM);
	ImGui::Text("VRAM Usage: %fGb", info.gpuVRAMUsage);
	ImGui::Text("VRAM Available: %fGb", info.gpuVRAMAV);
	ImGui::Text("VRAM Reserved: %fGb", info.gpuVRAMReserve);

	ImGui::End();
	
	MainMenuBar();

}

void EditorLayer::FrameHistogram()
{
	if (m_MSLog.size() > 64)
		m_MSLog.erase(0);

	if (m_FPSLog.size() > 64)
		m_FPSLog.erase(0);

	m_MSLog.push_back(1000.0f / ImGui::GetIO().Framerate);
	m_FPSLog.push_back(ImGui::GetIO().Framerate);

	char title[25];
	sprintf_s(title, 25, "Framerate %.1f", m_FPSLog[m_FPSLog.size() - 1]);
	ImGui::PlotHistogram("##framerate", &m_FPSLog[0], m_FPSLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	sprintf_s(title, 25, "Frametime %.1f", m_MSLog[m_MSLog.size() - 1]);
	ImGui::PlotHistogram("##frametime", &m_MSLog[0], m_MSLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

}

void EditorLayer::OnEvent(Wiwa::Event& event)
{
}

void EditorLayer::MainMenuBar()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("GuiDemo"))
			m_ShowDemo = !m_ShowDemo;
		if (ImGui::MenuItem("Documentation"))
			Wiwa::Application::Get().RequestBrowser("https://github.com/ZaroDev/WiwaEngine/wiki");
		if (ImGui::MenuItem("Report a bug"))
			Wiwa::Application::Get().RequestBrowser("https://github.com/ZaroDev/WiwaEngine/issues");

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	static bool show = true;
	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&show);
}
