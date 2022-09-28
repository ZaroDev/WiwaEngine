#include "ScenePanel.h"

#include <imgui.h>
ScenePanel::ScenePanel()
	: Panel("Scene")
{
}

ScenePanel::~ScenePanel()
{
}

void ScenePanel::Draw()
{
	ImGui::Begin(name, &active);
	ImGui::End();
}
