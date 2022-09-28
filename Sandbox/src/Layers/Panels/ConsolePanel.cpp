#include "ConsolePanel.h"

#include "Wiwa/Application.h"
#include "Wiwa/ImGui/ImGuiLayer.h"
#include "Wiwa/ImGui/ImGuiLog.h"
#include <imgui.h>

ConsolePanel::ConsolePanel()
	: Panel("Console")
{
}

ConsolePanel::~ConsolePanel()
{
}

void ConsolePanel::Draw()
{
	ImGui::Begin(name, &active);

	ImGui::End();
	Wiwa::Application::Get().GetImGuiLayer()->GetLog().Draw(name, &active);
}
