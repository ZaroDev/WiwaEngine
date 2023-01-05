#pragma once
#include "Panel.h"

#include <Wiwa/utilities/render/Material.h>

class Wiwa::Uniform;
class MaterialPanel : public Panel
{
public:
	MaterialPanel(EditorLayer* instance);
	virtual ~MaterialPanel();

	void Draw() override;

	void OnEvent(Wiwa::Event& e) override;
	bool OnMaterialChange(MaterialChangeEvent& e);
	void RenderUniform(Wiwa::Uniform* uniform);
private:
	Wiwa::Material* m_Material = nullptr;
	std::filesystem::path s_Path;
};