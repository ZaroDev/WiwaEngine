#include "wipch.h"
#include "AssetsPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Resources.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Image.h>
#include <Wiwa/utilities/render/shaders/Shader.h>
#include <Wiwa/utilities/json/JSONDocument.h>
#include "MaterialPanel.h"
#include "../EditorLayer.h"
#include "../../Utils/EditorUtils.h"


static const std::filesystem::path s_AssetsPath = "Assets";
AssetsPanel::AssetsPanel(EditorLayer* instance)
	: Panel("Assets", instance), m_CurrentPath("Assets")
{
	m_Directory.path = "Assets";
	Update();
	ResourceId folderId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/folder_icon.png");
	ResourceId fileId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/file_icon.png");
	ResourceId backId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/back_icon.png");
	ResourceId matId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/material_icon.png");

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
				DirectorySpecs*dir = new DirectorySpecs();
				dir->path = p.path();
				m_Directory.directories.push_back(dir);
				for (auto &p1 : std::filesystem::directory_iterator(dir->path))
				{
					UpdateDir(p1, dir);
				}
			}
			else
			{
				FileSpecs f;
				f.path = p.path();
				f.size = p.file_size();
				m_Directory.files.push_back(f);

				CheckImport(p);
			}
		}

		lastWriteTime = lastAbsoluteDirTime;
	}
}
void AssetsPanel::UpdateDir(const std::filesystem::directory_entry &p1, DirectorySpecs*dir)
{
	if (p1.is_directory())
	{
		DirectorySpecs*d = new DirectorySpecs();
		d->path = p1.path();
		dir->directories.push_back(d);
		for (auto &p2 : std::filesystem::directory_iterator(d->path))
		{
			UpdateDir(p2, d);
		}
	}
	else
	{
		FileSpecs f;
		f.path = p1.path();
		f.size = p1.file_size();
		dir->files.push_back(f);

		CheckImport(p1);
	}
}
void AssetsPanel::CheckImport(const std::filesystem::directory_entry& path)
{
	std::string p = path.path().string().c_str();
	Wiwa::Resources::standarizePath(p);
	if (ImageExtensionComp(path.path()))
	{
		Wiwa::ImageSettings settings;
		Wiwa::Resources::LoadMeta<Wiwa::Image>(p.c_str(), &settings);
		Wiwa::Resources::CreateMeta<Wiwa::Image>(p.c_str(), &settings);
		Wiwa::Resources::Import<Wiwa::Image>(p.c_str());
	}
	else if (ModelExtensionComp(path.path()))
	{
		Wiwa::ModelSettings settings;
		Wiwa::Resources::LoadMeta<Wiwa::Model>(p.c_str(), &settings);
		Wiwa::Resources::CreateMeta<Wiwa::Model>(p.c_str(), &settings);
		Wiwa::Resources::Import<Wiwa::Model>(p.c_str(), &settings);
	}
	else if (ShaderExtensionComp(path.path()))
	{
		p = p.substr(0, p.size() - 3);
		Wiwa::ResourceId id = Wiwa::Resources::Load<Wiwa::Shader>(p.c_str());
		Wiwa::Resources::CreateMeta<Wiwa::Shader>(p.c_str());
		Wiwa::Resources::Import<Wiwa::Shader>(p.c_str(), Wiwa::Resources::GetResourceById<Wiwa::Shader>(id));
	}
	else if (MaterialExtensionComp(path.path()))
	{
		Wiwa::Resources::CreateMeta<Wiwa::Material>(p.c_str());
		Wiwa::Resources::Import<Wiwa::Material>(p.c_str());
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
		TopBar();



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
					m_SelectedEntry = directoryEntry;
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
					if (m_SelectedEntry.exists())
					{
						ImGui::Separator();

						ImGui::TextDisabled(m_SelectedEntry.path().filename().string().c_str());
						if (ImGui::MenuItem("Delete"))
						{
							if (m_SelectedEntry.is_directory())
								_rmdir(m_SelectedEntry.path().string().c_str());
							else
								remove(m_SelectedEntry.path().string().c_str());
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

void AssetsPanel::TopBar()
{
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

	if (ImGui::Button("Create"))
		ImGui::OpenPopup("Create");
	int id = 0;
	if (ImGui::BeginPopup("Create"))
	{
		{
			ImGui::PushID(id++);
			ImGui::Text("Folder");
			static char buffer[64] = { 0 };
			ImGui::Text("Folder name");
			ImGui::InputText("##inputfolder", buffer, IM_ARRAYSIZE(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				std::filesystem::path path = m_CurrentPath;
				std::filesystem::path dir = buffer;
				path /= dir;
				if (_mkdir(path.string().c_str()) == -1)
					WI_ERROR("Folder can't be created");
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			ImGui::PopID();
		}
		{
			ImGui::PushID(id++);
			static char buffer[64] = { 0 };
			ImGui::Text("Material");
			ImGui::Text("Material name");
			ImGui::InputText("##inputfolder", buffer, IM_ARRAYSIZE(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				std::filesystem::path path = m_CurrentPath;
				std::filesystem::path dir = buffer;
				path /= dir;
				std::string file = path.string() + ".wimaterial";

				Wiwa::Material material;

				Wiwa::Material::SaveMaterial(file.c_str(), &material);

				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			ImGui::PopID();
		}
		{
			ImGui::PushID(id++);
			static char buffer[64] = { 0 };
			ImGui::Text("Shader");
			ImGui::Text("Shader name");
			ImGui::InputText("##inputfolder", buffer, IM_ARRAYSIZE(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				std::filesystem::path path = m_CurrentPath;
				std::filesystem::path dir = buffer;
				path /= dir;
				std::string file = path.string();
				
				Wiwa::Shader::CreateDefault(file.c_str());
				Wiwa::Shader* shader = new Wiwa::Shader();
				shader->setPath(file.c_str());
				Wiwa::Resources::Import<Wiwa::Shader>(file.c_str(), shader);
				Wiwa::Resources::Load<Wiwa::Shader>(file.c_str());
				
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(200.0f);
	ImGui::SliderFloat("##size", &m_ButtonSize, 0.5f, 2.0f);
	ImGui::PopItemWidth();
	ImGui::Separator();
}

void AssetsPanel::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Wiwa::WindowDropEvent>({&AssetsPanel::OnDragAndDrop, this});
}

bool AssetsPanel::OnDragAndDrop(Wiwa::WindowDropEvent& e)
{
	for (size_t i = 0; i < e.GetCount(); i++)
	{
		std::filesystem::path from = e.GetPaths()[i];
		std::filesystem::path to = m_CurrentPath;
		to /= from.filename();
		if(std::filesystem::is_directory(from))
		{
			if (_mkdir(from.string().c_str()) == -1)
			{
				WI_ERROR("Couldn't create directory at {0}", from.string().c_str());
			}
		}
		else
		{
			std::filesystem::copy_file(from, to);
		}
	}
	return true;
}

void AssetsPanel::DisplayNode(DirectorySpecs*directoryEntry)
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
