#pragma once

#include "Panel.h"

class HierarchyPanel : public Panel
{
public:
	HierarchyPanel(EditorLayer* instance);
	virtual ~HierarchyPanel();

	void Draw() override;
};