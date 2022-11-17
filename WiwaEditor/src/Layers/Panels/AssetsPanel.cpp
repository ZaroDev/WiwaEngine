#include "wipch.h"
#include "AssetsPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Resources.h>
#include <Wiwa/utilities/json/JSONDocument.h>
#include "MaterialPanel.h"
#include "../EditorLayer.h"


static const std::filesystem::path s_AssetsPath = "Assets";
AssetsPanel::AssetsPanel(EditorLayer* instance)
	: Panel("Assets", instance), m_CurrentPath("Assets")
{
	m_Directory.path = "Assets";

	ResourceId folderId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/folder_icon.png");
	ResourceId fileId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/file_icon.png");
	ResourceId backId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/back_icon.png");
	ResourceId matId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/material_icon.png");

	m_FolderIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(folderId)->GetTextureId();
	m_FileIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(fileId)->GetTextureId();
	m_BackIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(backId)->GetTextureId();
	m_MaterialIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(matId)->GetTextureId();
}

AssetsPanel::~AssetsPanel()
{
	m_Directory.directories.clear();
	m_Directory.files.clear();
}

void AssetsPanel::Update()
{
	auto lastAbsoluteDirTime = std::filesystem::last_write_time(m_CurrentPath);
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
	ImGui::Begin(name, &active);

	

	if (ImGui::BeginTable("##content_browser", 2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextColumn();
		//Folder browser
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

		//Back button
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton((ImTextureID)m_BackIcon, { 16, 16 }))
		{
			if (m_CurrentPath != std::filesystem::path(m_Directory.path))
				m_CurrentPath = m_CurrentPath.parent_path();
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::AlignTextToFramePadding();
		ImGui::Text(m_CurrentPath.string().c_str());
		ImGui::SameLine();
		//Create a folder button
		if (ImGui::Button("Create a folder"))
		{
			ImGui::OpenPopup("Creating a folder##folder");
		}
		ImGui::SameLine();
		if (ImGui::Button("Create a material"))
		{
			ImGui::OpenPopup("Creating a material##material");
		}
		bool open = true;
		if (ImGui::BeginPopupModal("Creating a folder##folder", &open))
		{
			static char buffer[64] = { 0 };
			ImGui::Text("Folder name");
			ImGui::InputText("##inputfolder", buffer, IM_ARRAYSIZE(buffer));
			if (ImGui::Button("Create"))
			{
				std::filesystem::path path = m_CurrentPath;
				std::filesystem::path dir = buffer;
				path /= dir;
				if (_mkdir(path.string().c_str()) == -1)
					WI_ERROR("Folder can't be created");
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		open = true;
		if (ImGui::BeginPopupModal("Creating a material##material", &open))
		{
			static char buffer[64] = { 0 };
			ImGui::Text("Material name");
			ImGui::InputText("##inputfolder", buffer, IM_ARRAYSIZE(buffer));
			if (ImGui::Button("Create"))
			{
				std::filesystem::path path = m_CurrentPath;
				std::filesystem::path dir = buffer;
				path /= dir;
				std::string file = path.string() + ".wimaterial";
				Wiwa::JSONDocument matFile;
				matFile.AddMember("texture", "");
				matFile.AddMember("colorR", 1.0f);
				matFile.AddMember("colorG",1.0f);
				matFile.AddMember("colorB",1.0f);
				matFile.AddMember("colorA",1.0f);
				matFile.AddMember("diffuseR", 1.0f);
				matFile.AddMember("diffuseG", 1.0f);
				matFile.AddMember("diffuseB", 1.0f);
				matFile.AddMember("specularR", 1.0f);
				matFile.AddMember("specularG", 1.0f);
				matFile.AddMember("specularB", 1.0f);
				matFile.AddMember("shininess", 0.1f);
				matFile.AddMember("type", 0);
				matFile.save_file(file.c_str());
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(200.0f);
		ImGui::SliderFloat("##size", &m_ButtonSize, 0.5f, 2.0f);
		ImGui::PopItemWidth();
		ImGui::Separator();
		//Assets display
		float padding = 16.0f * m_ButtonSize;
		float thumbnailSize = 64.0f * m_ButtonSize;
		float cellSize = (thumbnailSize + padding);
		
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;
		if (ImGui::BeginTable("##assets", columnCount))
		{
			int id = 0;
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentPath))
			{
				if (directoryEntry.path().extension() == ".meta")
					continue;
				ImGui::TableNextColumn();
				ImGui::PushID(id++);

				const auto& path = directoryEntry.path();
				auto relativePath = std::filesystem::relative(directoryEntry.path(), s_AssetsPath);
				std::string filenameString = relativePath.filename().string();
				
				ImTextureID texID = directoryEntry.is_directory() ? m_FolderIcon : m_FileIcon;
				if (directoryEntry.path().extension() == ".png")
				{
					ResourceId pngID = Wiwa::Resources::Load<Wiwa::Image>(path.string().c_str());
					texID = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(pngID)->GetTextureId();
				}
				if (directoryEntry.path().extension() == ".wimaterial")
					texID = m_MaterialIcon;

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::ImageButton(texID, { thumbnailSize, thumbnailSize }))
				{
					if (directoryEntry.path().extension() == ".wimaterial")
					{
						ResourceId matId = Wiwa::Resources::Load<Wiwa::Material>(path.string().c_str());
						MaterialChangeEvent event(matId);
						Action<Wiwa::Event&> action = { &EditorLayer::OnEvent, instance };
						action(event);
					}
				}

				ImGui::PopStyleColor();

				//Drag and drop
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = directoryEntry.path().c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}

				//Either opening a file or a folder
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
						m_CurrentPath /= path.filename();
					else
						Wiwa::Application::Get().OpenDir(path.string().c_str());
				}
				if (ImGui::BeginPopupContextWindow("Assets context window"))
				{
					if (ImGui::MenuItem("Find in explorer"))
					{
						Wiwa::Application::Get().OpenDir(m_CurrentPath.string().c_str());
					}
					if (directoryEntry.is_directory())
					{
						if (ImGui::MenuItem("Delete"))
						{
							std::filesystem::path p = m_CurrentPath;
							p /= path.filename();
							if (_rmdir(p.string().c_str()) == -1)
								WI_ERROR("Folder can't be destroyed");
						}
					}
					ImGui::EndPopup();
				}
				
				ImGui::TextWrapped(filenameString.c_str());

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
		ImGui::EndTable();
	}

	//Assets context window
	ImGui::End();
}

void AssetsPanel::DisplayNode(Directory *directoryEntry)
{
	const auto &path = directoryEntry->path;

	// ImGui::Text("%s", path.string().c_str());
	// auto relativePath = std::filesystem::relative(directoryEntry.path(), s_EditorPath);
	std::string filenameString = path.filename().string();
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	bool isDir = std::filesystem::is_directory(path);
	if (isDir)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::AlignTextToFramePadding();
		if (!directoryEntry->directories.empty())
		{
			bool open = ImGui::TreeNodeEx(filenameString.c_str(), base_flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
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
			bool open = ImGui::TreeNodeEx(filenameString.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
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
