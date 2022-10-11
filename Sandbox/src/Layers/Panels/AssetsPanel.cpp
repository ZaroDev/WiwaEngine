#include "AssetsPanel.h"

#include <imgui.h>
#include "wipch.h"
#include <Wiwa/Application.h>
#include <Wiwa/Resources.h>
#include <optick.h>
#include <format>

static const std::filesystem::path s_AssetsPath = "Assets";
AssetsPanel::AssetsPanel()
	: Panel("Assets"), m_CurrentPath("Assets")
{
	m_Directory.path = "Assets";

	ResourceId folderId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/folder_icon.png");
	ResourceId fileId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/file_icon.png");

	m_FolderIcon = (ImTextureID)Wiwa::Resources::GetResourceById<Wiwa::Image>(folderId)->GetTextureId();
	m_FileIcon = (ImTextureID)Wiwa::Resources::GetResourceById<Wiwa::Image>(fileId)->GetTextureId();
}

AssetsPanel::~AssetsPanel()
{
	m_Directory.directories.clear();
	m_Directory.files.clear();
}

void AssetsPanel::Update()
{
	auto lastAbsoluteDirTime = std::filesystem::last_write_time(m_Directory.path);
	if (lastWriteTime != lastAbsoluteDirTime)
	{
		m_Directory.directories.clear();
		m_Directory.files.clear();
		for (auto &p : std::filesystem::directory_iterator(m_Directory.path))
		{
			if (p.is_directory())
			{
				Directory *dir = new Directory();
				dir->path = p.path();
				m_Directory.directories.push_back(dir);
				for (auto &p1 : std::filesystem::directory_iterator(dir->path))
				{
					UpdateDir(p1, dir);
				}
			}
			else
			{
				File f;
				f.path = p.path();
				f.size = p.file_size();
				m_Directory.files.push_back(f);
			}
		}

		lastWriteTime = lastAbsoluteDirTime;
	}
}
void AssetsPanel::UpdateDir(const std::filesystem::directory_entry &p1, Directory *dir)
{
	if (p1.is_directory())
	{
		Directory *d = new Directory();
		d->path = p1.path();
		dir->directories.push_back(d);
		for (auto &p2 : std::filesystem::directory_iterator(d->path))
		{
			UpdateDir(p2, d);
		}
	}
	else
	{
		File f;
		f.path = p1.path();
		f.size = p1.file_size();
		dir->files.push_back(f);
	}
}
void AssetsPanel::Draw()
{

	OPTICK_EVENT();
	ImGui::Begin(name, &active);

	/*bool op = false;*/
	// ImGui::Text(m_SearchPath.string().c_str());
	if (ImGui::BeginTable("##content_browser", 2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextColumn();
		if (ImGui::BeginTable("##folder_browser", 1))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			for (auto &path : m_Directory.directories)
			{
				DisplayNode(path);
			}
			ImGui::EndTable();
		}
		ImGui::TableNextColumn();

		if (m_CurrentPath != std::filesystem::path(m_Directory.path))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}

		ImGui::Text(m_CurrentPath.string().c_str());

		float padding = 16.0f;
		float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		if (ImGui::BeginTable("##assets", columnCount))
		{
			int id = 0;
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentPath))
			{
				ImGui::TableNextColumn();
				ImGui::PushID(id++);

				const auto& path = directoryEntry.path();
				auto relativePath = std::filesystem::relative(directoryEntry.path(), s_AssetsPath);
				std::string filenameString = relativePath.filename().string();

				ImTextureID texID = directoryEntry.is_directory() ? m_FolderIcon : m_FileIcon;

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((ImTextureID)texID, { thumbnailSize, thumbnailSize });
				ImGui::PopStyleColor();

				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = relativePath.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
						m_CurrentPath /= path.filename();
					else
						Wiwa::Application::Get().OpenDir(path.string().c_str());
				}
				
				ImGui::TextWrapped(filenameString.c_str());

				ImGui::PopID();
			}

			ImGui::EndTable();
		}
		ImGui::EndTable();
	}

	ImGui::End();
}

void AssetsPanel::DisplayNode(Directory *directoryEntry)
{
	const auto &path = directoryEntry->path;

	// ImGui::Text("%s", path.string().c_str());
	// auto relativePath = std::filesystem::relative(directoryEntry.path(), s_EditorPath);
	std::string filenameString = path.filename().string();
	bool isDir = std::filesystem::is_directory(path);
	if (isDir)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::AlignTextToFramePadding();
		if (!directoryEntry->directories.empty())
		{
			char str[25];
			sprintf_s(str, 25, "##%s", filenameString.c_str());
			bool open = ImGui::TreeNodeEx(str);
			ImGui::SameLine();
			if (ImGui::Button(filenameString.c_str()))
			{
				m_CurrentPath = path;
			}
			if (open)
			{
				for (auto &p : directoryEntry->directories)
				{
					DisplayNode(p);
				}
				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::Button(filenameString.c_str()))
			{
				m_CurrentPath = path;
			}
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
	}
}
