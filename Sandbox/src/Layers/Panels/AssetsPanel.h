#pragma once

#include "Panel.h"

class AssetsPanel : public Panel
{
public:
	AssetsPanel();
	virtual ~AssetsPanel();

	void Draw() override;
};