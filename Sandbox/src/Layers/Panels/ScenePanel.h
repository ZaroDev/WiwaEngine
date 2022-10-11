#pragma once

#include "Panel.h"

struct ShadingView;


class ScenePanel : public Panel
{
public:
	ScenePanel();
	virtual ~ScenePanel();

	void Draw() override;

private:
	std::vector<ShadingView*> m_Shadings;
};
