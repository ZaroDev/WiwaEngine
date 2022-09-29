#pragma once


#include "Panel.h"

class PlayPanel : public Panel
{
public:
	PlayPanel();
	virtual ~PlayPanel();

	void Draw() override;
};