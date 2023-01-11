#pragma once

#include "Panel.h"

class CamerasPanel : public Panel
{
public:
	CamerasPanel(EditorLayer* instance);
	virtual ~CamerasPanel();

	void Draw() override;
	void Update() override;
};