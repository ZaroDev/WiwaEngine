#pragma once

#include "Panel.h"

#include <filesystem>

class AssetsPanel : public Panel
{
public:
	AssetsPanel();
	virtual ~AssetsPanel();

	void Draw() override;

private:
	std::filesystem::path m_CurrentPath;
};