#pragma once

#include "Panel.h"

#include <filesystem>
#include <Wiwa/events/Event.h>
#include <Wiwa/events/ApplicationEvent.h>
#include <FileWatch.h>
#include <memory>

typedef void* ImTextureID;

class AssetsPanel : public Panel
{
public:
	AssetsPanel(EditorLayer* instance);
	virtual ~AssetsPanel();

	void Draw() override;
	void OnEvent(Wiwa::Event& e) override;
	bool OnDragAndDrop(Wiwa::WindowDropEvent& e);

private:
	void InitImports(const std::filesystem::path& path);
	void UpdateImports(const std::filesystem::directory_entry& path);
	void DisplayNode(std::filesystem::directory_entry directoryEntry);
	void TopBar();
	static void CheckMeta(const std::filesystem::path& path);
	static void OnFolderEvent(const std::filesystem::path& path, const filewatch::Event change_type);
	static void DeleteFileAssets(std::filesystem::path& assetsPath);
	static void CheckImport(const std::filesystem::path& path);
	
	static void SubmitToAssetsThread(const std::function<void()> func);
	static void ExecuteAssetsThreadQueue();
private:
	ImTextureID m_FileIcon;
	ImTextureID m_FolderIcon;
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

	std::unique_ptr<filewatch::FileWatch<std::filesystem::path>> watcher;
};