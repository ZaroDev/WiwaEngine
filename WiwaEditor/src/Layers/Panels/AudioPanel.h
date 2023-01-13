#pragma once

#include "Panel.h"

class AudioPanel : public Panel
{
public:
	AudioPanel(EditorLayer* instance);
	virtual ~AudioPanel();

	void Draw() override;
};