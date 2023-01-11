#pragma once

#include "Panel.h"

#include "Wiwa/utilities/render/shaders/Shader.h"

class ShaderPanel : public Panel 
{
public:
	ShaderPanel(EditorLayer* instance);
	virtual ~ShaderPanel();

	void Draw() override;

	void DrawField(Wiwa::UniformField* field, const char*& nameToDelete);
private:
	Wiwa::Shader* m_Shader = nullptr;
	std::string m_ShaderPath;
};