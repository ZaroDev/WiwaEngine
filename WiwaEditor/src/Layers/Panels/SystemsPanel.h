#pragma once

#include "Panel.h"


class SystemsPanel : public Panel
{
public:
	SystemsPanel(EditorLayer* instance);
	virtual ~SystemsPanel();

	void Draw() override;
};