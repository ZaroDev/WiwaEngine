#pragma once
#include "Panel.h"

#include <Wiwa/utilities/render/Material.h>

class MaterialPanel : public Panel
{
public:
	MaterialPanel(EditorLayer* instance);
	virtual ~MaterialPanel();

	void Draw() override;

	void OnEvent(Wiwa::Event& e) override;
	bool OnMaterialChange(MaterialChangeEvent& e);
private:
	Wiwa::Material* m_Material = nullptr;
	std::filesystem::path s_Path;
};