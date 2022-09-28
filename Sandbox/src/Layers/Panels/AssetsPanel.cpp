#include "AssetsPanel.h"

#include <imgui.h>

AssetsPanel::AssetsPanel()
	: Panel("Assets")
{
}

AssetsPanel::~AssetsPanel()
{
}

void AssetsPanel::Draw()
{
	ImGui::Begin(name, &active);
	ImGui::End();
}
