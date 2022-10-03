#pragma once

#include "Panel.h"

#include <filesystem>

typedef size_t ResourceId;

class AssetsPanel : public Panel
{
public:
	AssetsPanel();
	virtual ~AssetsPanel();

	void Draw() override;

	void DisplayNode(const std::filesystem::directory_entry& directoryEntry);

private:
	std::filesystem::path m_CurrentPath;
	ResourceId m_FileIcon;
	ResourceId m_FolderIcon;
};