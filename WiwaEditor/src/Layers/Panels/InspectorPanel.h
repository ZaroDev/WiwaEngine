#pragma once

#include "Panel.h"

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/containers/Array.h>

#include <Wiwa/utilities/math/Vector3f.h>

class InspectorPanel : public Panel
{
private:
	void DrawComponent(size_t componentId);
	void DrawField(unsigned char* data, const Field& field);

	
public:
	InspectorPanel(EditorLayer* instance);
	virtual ~InspectorPanel();

	void Draw() override;
	void Update() override;

	void OnEvent(Wiwa::Event&) override;
	bool OnEntityChangeEvent(EntityChangeEvent& e);

	inline bool GetCurrentEntity(size_t& id)
	{
		id = m_CurrentID;
		return m_EntitySet;
	}
private:
	
	size_t m_CurrentID = 0;
	bool m_EntitySet = false;
};