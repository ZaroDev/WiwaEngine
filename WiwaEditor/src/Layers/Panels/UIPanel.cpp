#include <wipch.h>
#include "UIPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>

UIPanel::UIPanel(EditorLayer* instance) : Panel("UI", instance)
{
}

UIPanel::~UIPanel()
{
}

void UIPanel::Draw()
{
	ImGui::Begin(name, &active);

	ImGui::End();
}