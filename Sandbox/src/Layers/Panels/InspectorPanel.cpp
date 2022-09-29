#include "InspectorPanel.h"

#include <imgui.h>

InspectorPanel::InspectorPanel()
	: Panel("Inspector")
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::Draw()
{
	ImGui::Begin(name, &active);
	ImGui::End();
}
