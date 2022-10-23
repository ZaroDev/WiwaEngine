#include "ConsolePanel.h"

#include "Wiwa/Application.h"
#include "Wiwa/ImGui/ImGuiLayer.h"
#include "Wiwa/ImGui/ImGuiLog.h"
#include <imgui.h>

ConsolePanel::ConsolePanel(EditorLayer* instance)
	: Panel("Console", instance)
{
}

ConsolePanel::~ConsolePanel()
{
}

void ConsolePanel::Draw()
{
	ImGui::Begin(name, &active);

	ImGui::End();
	Wiwa::Application::Get().GetImGuiLayer().GetConsole().Draw(name, &active);

	ImGui::Begin("Log", &active);
	ImGui::End();
	Wiwa::Application::Get().GetImGuiLayer().GetLog().Draw("Log", &active);
}
