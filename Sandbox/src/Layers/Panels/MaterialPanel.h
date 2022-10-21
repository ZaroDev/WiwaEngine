#pragma once
#include "Panel.h"

#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/Events/RenderEvents.h>

class MaterialPanel : public Panel
{
public:
	MaterialPanel();
	virtual ~MaterialPanel();

	void Draw() override;
	void OnEvent(Wiwa::Event& e) override;

	bool OnMaterialChange(Wiwa::MaterialChange& e);
private:
	inline static Wiwa::Material* s_Material;
	inline static std::filesystem::path s_Path;
	inline static std::string s_TexturePath;
};