#pragma once

#include "Panel.h"
typedef void* ImTextureID;

#include <Wiwa/ecs/EntityManager.h>
#include <imgui.h>

class HierarchyPanel : public Panel
{
public:
	HierarchyPanel(EditorLayer* instance);
	virtual ~HierarchyPanel();

	void Draw() override;

	void CreateNode(const EntityId& eid, const char* entName, ImGuiTextFilter& filter, Wiwa::EntityManager& entityManager);

private:
	int m_CurrentID = -1;
};