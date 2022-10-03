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
	uint32_t m_FileIcon;
	uint32_t m_FolderIcon;
};