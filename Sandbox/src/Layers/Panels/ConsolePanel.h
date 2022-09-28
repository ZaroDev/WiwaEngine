#pragma once
#include "Panel.h"

class ConsolePanel : public Panel
{
public:
	ConsolePanel();
	virtual ~ConsolePanel();

	void Draw() override;
};