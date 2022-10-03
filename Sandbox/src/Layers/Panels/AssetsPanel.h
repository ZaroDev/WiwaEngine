#pragma once

#include "Panel.h"

#include <filesystem>

class AssetsPanel : public Panel
{
public:
	AssetsPanel();
	virtual ~AssetsPanel();

	void Draw() override;

	void DisplayNode(const std::filesystem::directory_entry& directoryEntry);

private:
	std::filesystem::path m_CurrentPath;

};