#include "EnvoirmentPanel.h"

#include <Wiwa\scene\SceneManager.h>
#include <Wiwa\utilities\render\LightManager.h>

EnvoirmentPanel::EnvoirmentPanel(EditorLayer* instance)
	: Panel("Envoirment", "", instance)
{
}

EnvoirmentPanel::~EnvoirmentPanel()
{
}

void EnvoirmentPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	ImGui::Text("Nº Lights %i", Wiwa::SceneManager::getActiveScene()->GetLightManager().GetLightsSize());

	ImGui::End();
}
