#pragma once

#include "Panel.h"
typedef void* ImTextureID;

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/events/ApplicationEvent.h>

class HierarchyPanel : public Panel
{
public:
	HierarchyPanel(EditorLayer* instance);
	virtual ~HierarchyPanel();

	void Draw() override;

	void DrawAddMenu(Wiwa::EntityManager& entityManager);

	void CreateNode(const EntityId& eid, const char* entName, ImGuiTextFilter& filter, Wiwa::EntityManager& entityManager);
	bool OnEntityChange(EntityChangeEvent& e);
	bool OnSceneChange(Wiwa::SceneChangeEvent& e);
	void OnEvent(Wiwa::Event& e) override;
private:
	int m_CurrentID = -1;
};