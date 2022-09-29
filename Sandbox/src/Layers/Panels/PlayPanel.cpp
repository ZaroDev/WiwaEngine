#include "PlayPanel.h"
#include <imgui.h>

PlayPanel::PlayPanel()
	: Panel("Toolbar")
{
}

PlayPanel::~PlayPanel()
{
}

void PlayPanel::Draw()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("##toolbar", nullptr, flags);
	if (ImGui::Button("play"))
	{

	}
	ImGui::End();
}
