#pragma once

#include "Panel.h"
typedef void* ImTextureID;

class HierarchyPanel : public Panel
{
public:
	HierarchyPanel(EditorLayer* instance);
	virtual ~HierarchyPanel();

	void Draw() override;

private:
	int m_CurrentID = -1;
	ImTextureID m_AddIcon;
};