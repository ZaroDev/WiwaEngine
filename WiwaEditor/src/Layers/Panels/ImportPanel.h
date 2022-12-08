#pragma once

#include "Panel.h"
#include <Wiwa/core/Resources.h>

class ImportPanel : public Panel
{
public:
	ImportPanel(EditorLayer* instance);
	virtual ~ImportPanel();

	void Draw() override;
	void ImageImport();
	void ModelImport();
private:
	std::filesystem::path m_CurrentPath = "";
	std::filesystem::path m_MetaPath = "";
	Wiwa::ModelSettings m_ModelSettings;
	Wiwa::ImageSettings m_ImageSettings;
};