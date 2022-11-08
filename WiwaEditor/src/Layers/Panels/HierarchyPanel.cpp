#include "HierarchyPanel.h"

#include <imgui.h>

#include <Wiwa/Application.h>
#include <Wiwa/ecs/EntityManager.h>
#include "InspectorPanel.h"

#include "../../Entities.h"
#include <Wiwa/Resources.h>

#include <Wiwa/ecs/systems/SpriteRenderer.h>

#include "ScenePanel.h"
#include "../EditorLayer.h"

#include "../../Utils/ImGuiWidgets.h"

HierarchyPanel::HierarchyPanel(EditorLayer* instance)
	: Panel("Hierarchy", instance)
{
	ResourceId addID = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/add_icon.png");
	m_AddIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(addID);
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
		ImGui::Separator();
		if (ImGui::BeginMenu("Primitives"))
		{
			if (ImGui::MenuItem("Create cube"))
			{
				CreateCube();
			}
			if (ImGui::MenuItem("Create plane"))
			{
				CreatePlane();
			}
			if (ImGui::MenuItem("Create sphere"))
			{
				CreateSphere();
			}
			if (ImGui::MenuItem("Create pyramid"))
			{
				CreatePyramid();
			}
			ImGui::EndMenu();
		}
		if (m_CurrentID >= 0)
		{
			ImGui::Separator();
			ImGui::Text(entityManager.GetEntityName(m_CurrentID));
			if (ImGui::MenuItem("Delete"))
			{
				entityManager.DestroyEntity(m_CurrentID);
				m_CurrentID = -1;
			}
		}
		ImGui::EndPopup();
	}

	if (ImGui::ImageButton(m_AddIcon, {20, 20}))
	{
		Wiwa::Application::Get().GetEntityManager().CreateEntity();
	}

	float width = ImGui::GetWindowContentRegionWidth();
	int id = 0;
	
	std::vector<EntityId>* entities = entityManager.GetEntitiesAlive();
	size_t count = entities->size();

	for (size_t i = 0; i < count; i++)
	{
		EntityId eid = entities->at(i);
		const char* entName = entityManager.GetEntityName(eid);
		
		ImGui::PushID(id++);
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if (eid == m_CurrentID)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		ImGui::TreeNodeEx(entName, flags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			EntityChangeEvent event((uint32_t)eid);
			Action<Wiwa::Event&> act = { &EditorLayer::OnEvent, instance};
			act(event);
			m_CurrentID = eid;
		}
		
		ImGui::PopID();
	}
	
	ImGui::End();
}