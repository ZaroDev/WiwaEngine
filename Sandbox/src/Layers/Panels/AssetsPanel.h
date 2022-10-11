#pragma once

#include "Panel.h"

#include <filesystem>

struct File
{
	std::filesystem::path path;
	uint16_t size;
};
struct Directory
{
	std::filesystem::path path;
	std::vector<Directory> directories;
	std::vector<File> files;
};

class AssetsPanel : public Panel
{
public:
	AssetsPanel();
	virtual ~AssetsPanel();


	void Update() override;

	//void UpdateDir(const std::filesystem::directory_entry& p);

	void Draw() override;

	void DisplayNode(Directory directoryEntry);

private:
	std::filesystem::path m_CurrentPath;
	uint32_t m_FileIcon;
	uint32_t m_FolderIcon;
	Directory m_Directory;
	std::filesystem::file_time_type lastWriteTime;
};