#pragma once
#include "Panel.h"

class ConsolePanel : public Panel
{
public:
	ConsolePanel(EditorLayer* instance);
	virtual ~ConsolePanel();

	void Draw() override;
};