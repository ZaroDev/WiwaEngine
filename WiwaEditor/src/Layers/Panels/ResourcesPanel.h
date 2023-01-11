#pragma once

#include "Panel.h"

class ResourcesPanel : public Panel
{
public:
	ResourcesPanel(EditorLayer* instance);
	virtual ~ResourcesPanel();

	void Draw() override;
};