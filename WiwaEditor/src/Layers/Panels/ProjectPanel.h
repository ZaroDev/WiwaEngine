#pragma once

#include "Panel.h"

class ProjectPanel : public Panel
{
public:
	ProjectPanel(EditorLayer* instance);
	virtual ~ProjectPanel();

	void Draw() override;
};
