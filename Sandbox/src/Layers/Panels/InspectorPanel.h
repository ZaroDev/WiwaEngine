#pragma once

#include "Panel.h"


#include <Wiwa/Reflection.h>
#include <Wiwa/utilities/containers/Array.h>

class InspectorPanel : public Panel
{
public:
	InspectorPanel();
	virtual ~InspectorPanel();

	void Draw() override;
	void Update() override;

	Wiwa::Array<const Type*, 1> m_Components;
public:
	static inline void SetEntity(uint32_t entityID) { m_CurrentID = entityID; m_EntitySet = true; };
	static inline bool GetCurrentEntity(uint32_t& entityID) 
	{
		entityID = m_CurrentID;
		return m_EntitySet; 
	}
	static uint32_t m_CurrentID;
	static bool m_EntitySet;
};
