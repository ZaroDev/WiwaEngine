#pragma once

#include "Panel.h"

#include <filesystem>
#include <Wiwa/events/Event.h>
#include <Wiwa/events/ApplicationEvent.h>
#include <FileWatch.h>
#include <memory>

typedef void* ImTextureID;
struct FileSpecs
{
	std::filesystem::path path;
	uintmax_t size = 0;
};
struct DirectorySpecs
{
	std::filesystem::path path;
	std::vector<DirectorySpecs*> directories;
	std::vector<FileSpecs> files;
};

class AssetsPanel : public Panel
{
public:
	AssetsPanel(EditorLayer* instance);
	virtual ~AssetsPanel();

	void Update() override;
	void UpdateDir(const std::filesystem::directory_entry &p1, DirectorySpecs*dir);
	void CheckImport(const std::filesystem::directory_entry& p1);
	void Draw() override;
	void OnEvent(Wiwa::Event& e) override;
	bool OnDragAndDrop(Wiwa::WindowDropEvent& e);
private:
	void DisplayNode(DirectorySpecs* directoryEntry);
	void TopBar();
	static void OnAssetsFolderEvent(const std::string& path, const filewatch::Event change_type);
private:
	ImTextureID m_FileIcon;
	ImTextureID m_FolderIcon;
	ImTextureID m_BackIcon;
	ImTextureID m_MaterialIcon;
	 
	std::filesystem::path m_CurrentPath;
	std::filesystem::directory_entry m_SelectedEntry;
	std::filesystem::file_time_type lastWriteTime;
	float m_ButtonSize = 1.0f;
	std::unique_ptr<filewatch::FileWatch<std::string>> DirWatcher;
};