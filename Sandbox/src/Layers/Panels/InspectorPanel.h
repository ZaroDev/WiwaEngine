#pragma once

#include "Panel.h"

class InspectorPanel : public Panel
{
public:
	InspectorPanel();
	virtual ~InspectorPanel();

	void Draw() override;
	void Update() override;

	static inline void SetEntity(uint32_t entityID) { m_CurrentID = entityID; };

private:
	static uint32_t m_CurrentID;

};
