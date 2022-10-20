#pragma once
#include "Panel.h"

#include <Wiwa/utilities/render/Material.h>

class MaterialPanel : public Panel
{
public:
	MaterialPanel();
	virtual ~MaterialPanel();

	void Draw() override;

private:
	Wiwa::Material* m_Material;
};