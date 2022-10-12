#pragma once

#include "Panel.h"

struct ShadingView;


class ScenePanel : public Panel
{
public:
	ScenePanel();
	virtual ~ScenePanel();

	void Draw() override;

	inline void SetGizmoType(int type) { m_GizmoType = type; }

private:
	std::vector<ShadingView*> m_Shadings;

	int m_GizmoType = -1;
};
