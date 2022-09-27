#pragma once

#include "Panel.h"

class AboutPanel : public Panel
{
public:
	AboutPanel();
	virtual ~AboutPanel();

	void Draw() override;
};