#include "HierarchyPanel.h"

#include <imgui.h>

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include "InspectorPanel.h"

#include "../../Entities.h"
#include <Wiwa/core/Resources.h>

#include <Wiwa/ecs/systems/SpriteRenderer.h>

#include "ScenePanel.h"
#include "../EditorLayer.h"

#include "../../Utils/ImGuiWidgets.h"

HierarchyPanel::HierarchyPanel(EditorLayer* instance)
	: Panel("Hierarchy", instance)
{
	
}

HierarchyPanel::~HierarchyPanel()
{
}

void HierarchyPanel::Draw()
{
	Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
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
			ImGui::TextDisabled(entityManager.GetEntityName(m_CurrentID));
			if (ImGui::MenuItem("Create child"))
			{
				EntityId id = entityManager.CreateEntity("New entity", m_CurrentID);
				
			}

			if (ImGui::MenuItem("Delete"))
			{
				entityManager.DestroyEntity(m_CurrentID);
				m_CurrentID = -1;
			}
		}
		ImGui::EndPopup();
	}

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	if (ImGui::Button("+"))
	{
		Wiwa::SceneManager::getActiveScene()->GetEntityManager().CreateEntity();
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();
	
	static ImGuiTextFilter filter;
	filter.Draw("##searchbar", 200.f);
	ImGui::Separator();

	std::vector<EntityId>* entities = entityManager.GetParentEntitiesAlive();
	size_t count = entities->size();
	int id = 0;
	for (size_t i = 0; i < count; i++) {
		ImGui::PushID(id++);
		EntityId eid = entities->at(i);
		const char* entName = entityManager.GetEntityName(eid);
		if (filter.PassFilter(entName))
		{
			CreateNode(eid, entName, filter,entityManager);
		}
		ImGui::PopID();
	}

	ImGui::End();
}

void HierarchyPanel::CreateNode(const EntityId& eid, const char* entName, ImGuiTextFilter& filter, Wiwa::EntityManager& entityManager)
{
	std::vector<EntityId>* childs = entityManager.GetEntityChildren(eid);
	if (!childs->empty())
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (eid == m_CurrentID)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		bool open = ImGui::TreeNodeEx(entName, flags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			EntityChangeEvent event((uint32_t)eid);
			Action<Wiwa::Event&> act = { &EditorLayer::OnEvent, instance };
			act(event);
			m_CurrentID = eid;
		}
		if (open)
		{
			int i = 0;
			for (EntityId id : *childs)
			{
				const char* name = entityManager.GetEntityName(id);
				std::string label = name;
				label += "##" + std::to_string(i++);
				if (filter.PassFilter(label.c_str()))
				{
					CreateNode(id, label.c_str(), filter, entityManager);
				}
			}
			ImGui::TreePop();
		}

	}
	else
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if (eid == m_CurrentID)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		bool open = ImGui::TreeNodeEx(entName, flags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			EntityChangeEvent event((uint32_t)eid);
			Action<Wiwa::Event&> act = { &EditorLayer::OnEvent, instance };
			act(event);
			m_CurrentID = eid;
		}
	}

}
