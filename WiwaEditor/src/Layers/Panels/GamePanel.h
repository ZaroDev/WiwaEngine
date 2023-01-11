#pragma once

#include "Panel.h"
#include <Wiwa/utilities/render/Camera.h>
class GamePanel : public Panel
{
public:
	GamePanel(EditorLayer* instance);
	virtual ~GamePanel();
	void Draw() override;
	void Update() override;
private:
	bool m_ShowStats = true;
};