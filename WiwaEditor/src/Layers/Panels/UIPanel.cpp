#include <wipch.h>
#include "UIPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>

UIPanel::UIPanel(EditorLayer* instance) 
	: Panel("UI", ICON_FK_TELEVISION, instance)
{
}

UIPanel::~UIPanel()
{
}

void UIPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	ImGui::End();
}