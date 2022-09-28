#pragma once

#include "Panel.h"

class HierarchyPanel : public Panel
{
public:
	HierarchyPanel();
	virtual ~HierarchyPanel();

	void Draw() override;
};