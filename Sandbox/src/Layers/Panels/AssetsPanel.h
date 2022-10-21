#pragma once

#include "Panel.h"

#include <filesystem>
typedef void* ImTextureID;
struct File
{
	std::filesystem::path path;
	uintmax_t size = 0;
};
struct Directory
{
	std::filesystem::path path;
	std::vector<Directory *> directories;
	std::vector<File> files;
};

class AssetsPanel : public Panel
{
public:
	AssetsPanel();
	virtual ~AssetsPanel();

	void Update() override;
	void Draw() override;

private:
	void DisplayNode(Directory *directoryEntry);
	void UpdateDir(const std::filesystem::directory_entry &p1, Directory *dir);

private:
	ImTextureID m_FileIcon;
	ImTextureID m_FolderIcon;
	ImTextureID m_BackIcon;
	Directory m_Directory;
	std::filesystem::path m_CurrentPath;
	std::filesystem::file_time_type lastWriteTime;
};