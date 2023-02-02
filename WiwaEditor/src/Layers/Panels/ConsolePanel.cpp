#include "ConsolePanel.h"

#include <Wiwa/core/Application.h>
#include "Wiwa/ImGui/ImGuiLayer.h"
#include "Wiwa/ImGui/ImGuiLog.h"
#include <imgui.h>

ConsolePanel::ConsolePanel(EditorLayer* instance)
	: Panel("Console", ICON_FK_COMMENT_O, instance)
{
}

ConsolePanel::~ConsolePanel()
{
}

void ConsolePanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	ImGui::End();
	Wiwa::Application::Get().GetImGuiLayer().GetConsole().Draw(iconName.c_str(), &active);

	ImGui::Begin(ICON_FK_TERMINAL "Log", &active);
	ImGui::End();
	Wiwa::Application::Get().GetImGuiLayer().GetLog().Draw(ICON_FK_TERMINAL "Log", &active);
}
