#pragma once

#include "Panel.h"

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/containers/Array.h>

#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/ecs/EntityManager.h>

class InspectorPanel : public Panel
{
private:
	bool DrawComponent(size_t componentId);
	void DrawField(unsigned char* data, const Field& field);

	void DrawMeshComponent(byte* data);
public:
	InspectorPanel(EditorLayer* instance);
	virtual ~InspectorPanel();

	void Draw() override;
	void Update() override;

	void OnEvent(Wiwa::Event&) override;
	bool OnEntityChangeEvent(EntityChangeEvent& e);

	inline bool GetCurrentEntity(int& id)
	{
		id = m_CurrentID;
		return m_EntitySet;
	}
private:
	
	int m_CurrentID = 0;
	bool m_EntitySet = false;
};