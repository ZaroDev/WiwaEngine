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
	std::unique_ptr<filewatch::FileWatch<std::filesystem::path>> watcher;
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
	static void CheckImport(const std::filesystem::path& p1);
	void Draw() override;
	void OnEvent(Wiwa::Event& e) override;
	bool OnDragAndDrop(Wiwa::WindowDropEvent& e);
private:
	void DisplayNode(DirectorySpecs* directoryEntry);
	void TopBar();

	static void OnFolderEvent(const std::filesystem::path& path, const filewatch::Event change_type, const std::filesystem::path& dir);
	static void SubmitToAssetsThread(const std::function<void()> func);
	static void ExecuteAssetsThreadQueue();
private:
	ImTextureID m_FileIcon;
	ImTextureID m_FolderIcon;
	ImTextureID m_BackIcon;
	ImTextureID m_MaterialIcon;
	ImTextureID m_ModelIcon;
	ImTextureID m_ScriptIcon;
	ImTextureID m_ShaderIcon;

	 
	std::filesystem::path m_CurrentPath;
	std::filesystem::directory_entry m_SelectedEntry;
	std::filesystem::file_time_type lastWriteTime;
	float m_ButtonSize = 1.0f;

	static std::vector<std::function<void()>> m_AssetsThreadQueue;
	static std::mutex m_AssetsThreadQueueMutex;
};