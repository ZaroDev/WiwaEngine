#pragma once

#include "Panel.h"

class PanelTemplate : public Panel 
{
public:
	PanelTemplate(EditorLayer* instance);
	virtual ~PanelTemplate();

	void Draw() override;
	void Update() override;

};
