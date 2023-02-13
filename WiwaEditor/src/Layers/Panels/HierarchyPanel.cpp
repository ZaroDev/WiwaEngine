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

#include "../../Utils/EditorUtils.h"

HierarchyPanel::HierarchyPanel(EditorLayer* instance)
	: Panel("Hierarchy", ICON_FK_LIST_UL, instance)
{
	
}

HierarchyPanel::~HierarchyPanel()
{
}

void HierarchyPanel::Draw()
{
	Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Begin(iconName.c_str(), &active);
	if (ImGui::BeginPopupContextWindow("Context Menu"))
	{
		DrawAddMenu(entityManager);
		ImGui::EndPopup();
	}
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	if (ImGui::Button(ICON_FK_PLUS))
	{
		ImGui::OpenPopup("Add menu");
	}
	if (ImGui::BeginPopup("Add menu"))
	{
		DrawAddMenu(entityManager);
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();
	
	static ImGuiTextFilter filter;
	filter.Draw("##searchbar", 200.f);
	ImGui::Separator();
	std::string sceneName = ICON_FK_CUBES " ";
	sceneName += Wiwa::SceneManager::getActiveScene()->getName();
	ImGui::Text(sceneName.c_str());
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

void HierarchyPanel::DrawAddMenu(Wiwa::EntityManager& entityManager)
{
	if (ImGui::MenuItem("Create New Entity"))
	{
		CreateNew3DEnt();
	}
	ImGui::Separator();
	if (ImGui::BeginMenu("Primitives"))
	{
		if (ImGui::MenuItem("Create cube"))
		{
			CreateCube();
		}
		/*if (ImGui::MenuItem("Create plane"))
		{
			CreatePlane();
		}*/
		if (ImGui::MenuItem("Create sphere"))
		{
			CreateSphere();
		}
		/*if (ImGui::MenuItem("Create pyramid"))
		{
			CreatePyramid();
		}*/
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Camera"))
	{
		Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
		float ar = res.w / (float)res.h;
		Wiwa::SceneManager::getActiveScene()->GetCameraManager().CreatePerspectiveCamera(60, ar);
	}
	if (ImGui::MenuItem("Light source"))
	{
		CreateLightSource();
	}
	if (m_CurrentID >= 0)
	{
		ImGui::Separator();
		ImGui::TextDisabled(entityManager.GetEntityName(m_CurrentID));
		if (ImGui::MenuItem("Create child"))
		{
			CreateNewChild(m_CurrentID);
		}
		if (entityManager.GetEntityParent(m_CurrentID) != m_CurrentID)
		{
			if (ImGui::MenuItem("Unparent"))
			{
				entityManager.SetParent(m_CurrentID, m_CurrentID);
			}
		}
		if (ImGui::BeginMenu("Reparent to"))
		{
			for(size_t i = 0; i < entityManager.GetEntitiesAlive()->size(); i++)
			{
				if (i == m_CurrentID)
					continue;
				const char* name = entityManager.GetEntityName(i);
				if (ImGui::MenuItem(name))
				{
					entityManager.SetParent(m_CurrentID, i);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Delete"))
		{
			entityManager.DestroyEntity(m_CurrentID);
			m_CurrentID = -1;
		}
	}
}

void HierarchyPanel::CreateNode(const EntityId& eid, const char* entName, ImGuiTextFilter& filter, Wiwa::EntityManager& entityManager)
{
	std::vector<EntityId>* childs = entityManager.GetEntityChildren(eid);
	std::string name = ICON_FK_CUBE " ";
	name += entName;
	if (!childs->empty())
	{
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (eid == m_CurrentID)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		
		bool open = ImGui::TreeNodeEx(name.c_str(), flags);
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
		bool open = ImGui::TreeNodeEx(name.c_str(), flags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			EntityChangeEvent event((uint32_t)eid);
			Action<Wiwa::Event&> act = { &EditorLayer::OnEvent, instance };
			act(event);
			m_CurrentID = eid;
		}
	}

}

bool HierarchyPanel::OnEntityChange(EntityChangeEvent& e)
{
	m_CurrentID = (int)e.GetResourceId();
	return false;
}

bool HierarchyPanel::OnSceneChange(Wiwa::SceneChangeEvent& e)
{
	m_CurrentID = -1;
	return false;
}

void HierarchyPanel::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<EntityChangeEvent>({ &HierarchyPanel::OnEntityChange, this });
	dispatcher.Dispatch<Wiwa::SceneChangeEvent>({ &HierarchyPanel::OnSceneChange, this });
}
