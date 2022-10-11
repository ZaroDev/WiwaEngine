#include "HierarchyPanel.h"

#include <imgui.h>

#include <Wiwa/Application.h>
#include <Wiwa/ecs/EntityManager.h>
#include "InspectorPanel.h"

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
	if (ImGui::BeginPopupContextWindow("Context Menu"))
	{
		if (ImGui::MenuItem("New Entity"))
		{
			WI_INFO("CREATING A NEW ENTITY");
			Wiwa::Application::Get().GetEntityManager().CreateEntity();
		}
		ImGui::EndPopup();
	}
	if(ImGui::Button("+"))
	{
		WI_INFO("CREATING A NEW ENTITY");
		Wiwa::Application::Get().GetEntityManager().CreateEntity();
	}
	float width = ImGui::GetWindowContentRegionWidth();
	for (size_t i = 0; i < Wiwa::Application::Get().GetEntityManager().GetEntityCount(); i++)
	{
		if (ImGui::Button("Entity!", ImVec2(width, 20)))
		{
			InspectorPanel::SetEntity(i);
		}
	}

	ImGui::End();
}
