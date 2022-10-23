#pragma once

#include "Panel.h"

class AboutPanel : public Panel
{
public:
	AboutPanel(EditorLayer* instance);
	virtual ~AboutPanel();

	void Draw() override;
};