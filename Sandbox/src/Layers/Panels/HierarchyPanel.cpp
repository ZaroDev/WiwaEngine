#include "HierarchyPanel.h"

#include <imgui.h>

#include <Wiwa/Application.h>
#include <Wiwa/ecs/EntityManager.h>
#include "InspectorPanel.h"

#include "../../Entities.h"
#include <Wiwa/Resources.h>

#include <Wiwa/ecs/systems/SpriteRenderer.h>

#include "ScenePanel.h"

HierarchyPanel::HierarchyPanel()
	: Panel("Hierarchy")
{
}

HierarchyPanel::~HierarchyPanel()
{
}

void HierarchyPanel::Draw()
{
	Wiwa::EntityManager& entityManager = Wiwa::Application::Get().GetEntityManager();
	ImGui::Begin(name, &active);
	if (ImGui::BeginPopupContextWindow("Context Menu"))
	{
		if (ImGui::MenuItem("Create New Entity"))
		{
			entityManager.CreateEntity("New entity");
		}
		//TODO: Remove test
		if(ImGui::MenuItem("Create 2D tree"))
		{
			ResourceId tree = Wiwa::Resources::Load<Wiwa::Image>("resources/images/tree.png");
			Wiwa::Image* spr = Wiwa::Resources::GetResourceById<Wiwa::Image>(tree);
			Wiwa::Size2i size = spr->GetSize();
			EntityId EntityMyTree = entityManager.CreateEntity("Arbolsito rechulon");
			entityManager.AddComponent<Wiwa::Transform2D>(EntityMyTree, { {0,0},0.f,{1.0,1.0} });
			entityManager.AddComponent<Wiwa::Sprite>(EntityMyTree, { {256,256}, tree,{size.w / 4, size.h / 4, size.w / 2, size.h / 2} });
		}
		if (ImGui::BeginMenu("Primitives"))
		{
			if (ImGui::MenuItem("Create cube"))
			{
				ScenePanel::SetMesh(CreateCube());
			}
			if (ImGui::MenuItem("Create plane"))
			{
				ScenePanel::SetMesh(CreatePlane());
			}
			if (ImGui::MenuItem("Create sphere"))
			{
				ScenePanel::SetMesh(CreateSphere());
			}
			if (ImGui::MenuItem("Create pyramid"))
			{
				ScenePanel::SetMesh(CreatePyramid());
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}
	if(ImGui::Button("+"))
	{
		Wiwa::Application::Get().GetEntityManager().CreateEntity();
	}
	float width = ImGui::GetWindowContentRegionWidth();
	int id = 0;
	for (size_t i = 0; i < Wiwa::Application::Get().GetEntityManager().GetEntityCount(); i++)
	{
		const char* entName = entityManager.GetEntityName(i);
		
		ImGui::PushID(id++);
		if (ImGui::Button(entName, ImVec2(width, 20)))
		{
			InspectorPanel::SetEntity((uint32_t)i);
		}
		ImGui::PopID();
	}

	ImGui::End();
}
