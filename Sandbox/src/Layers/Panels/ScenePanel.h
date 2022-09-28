#pragma once

#include "Panel.h"

class ScenePanel : public Panel
{
public:
	ScenePanel();
	virtual ~ScenePanel();

	void Draw() override;
};
