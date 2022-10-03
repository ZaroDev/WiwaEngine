#pragma once

#include "Panel.h"

struct ShadingView
{
	const char* name;
	bool active = false;

	ShadingView() = default;

	ShadingView(const char* Name, bool Active)
		: name(Name), active(Active){}
};


class ScenePanel : public Panel
{
public:
	ScenePanel();
	virtual ~ScenePanel();

	void Draw() override;

private:
	std::vector<ShadingView*> m_Shadings;
};
