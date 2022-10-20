#pragma once
#include "Panel.h"

#include <Wiwa/utilities/render/Material.h>

class MaterialPanel : public Panel
{
public:
	MaterialPanel();
	virtual ~MaterialPanel();

	void Draw() override;

	static void SetMaterial(const char* file);

private:
	inline static Wiwa::Material* s_Material;
	inline static std::filesystem::path s_Path;
	inline static std::string s_TexturePath;
};