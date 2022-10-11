#pragma once

#include "Panel.h"

class ShadingView;

class MeshViewPanel : public Panel
{
public:
	MeshViewPanel();
	virtual ~MeshViewPanel();

	void Draw() override;

private:
	std::vector<ShadingView*> m_Shadings;


};