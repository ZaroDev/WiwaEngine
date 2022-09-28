#include "HierarchyPanel.h"

#include <imgui.h>

HierarchyPanel::HierarchyPanel()
	: Panel("Hierarchy")
{
}

HierarchyPanel::~HierarchyPanel()
{
}

void HierarchyPanel::Draw()
{
	ImGui::Begin(name, &active);
	ImGui::End();
}
