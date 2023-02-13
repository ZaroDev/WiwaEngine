#pragma once

#include "Panel.h"

class EnvoirmentPanel : public Panel
{
public:
	EnvoirmentPanel(EditorLayer* instance);
	virtual ~EnvoirmentPanel();

	void Draw() override;
};