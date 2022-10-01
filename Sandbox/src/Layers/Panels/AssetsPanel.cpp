#include "AssetsPanel.h"

#include <imgui.h>
#include "wipch.h"
#include <Wiwa/Application.h>
#include <imgui_internal.h>

//TODO: Change withing projects
static const std::filesystem::path s_AssetsPath = "Assets";

AssetsPanel::AssetsPanel()
	: Panel("Assets"), m_CurrentPath(s_AssetsPath)
{
	//TODO: Load the icons textures
	
}

AssetsPanel::~AssetsPanel()
{
}

void AssetsPanel::Draw()
{

	ImGui::Begin(name, &active);

	
	/*bool op = false;*/
	//ImGui::Text(m_SearchPath.string().c_str());
	if (ImGui::BeginTable("##content_browser", 2, ImGuiTableFlags_Resizable))
	{
		static ImGuiTableFlags flags = ImGuiTableFlags_BordersOuterH  | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
		ImGui::TableNextColumn();
		if (ImGui::BeginTable("##folder_browser", 1, flags))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			for (auto& directoryEntry : std::filesystem::directory_iterator(s_AssetsPath))
			{
				DisplayNode(directoryEntry);
			}
			ImGui::EndTable();
		}
		ImGui::TableNextColumn();

		if (m_CurrentPath != std::filesystem::path(s_AssetsPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
			ImGui::SameLine();
		}

		ImGui::Text(m_CurrentPath.string().c_str());

		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
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

				//TODO: Add the ImGui::ImageButton(Texture, { thumbnailSize, thumbnailSize }, {0, 1}, {1, 0});
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::Button(filenameString.c_str(), { thumbnailSize, thumbnailSize });
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
					{
						//TODO: Open file
						//op = true;
						
						WI_INFO("Opening popup");
						Wiwa::Application::Get().OpenDir(path.string().c_str());
					}
					
				}
				ImGui::TextWrapped(filenameString.c_str());

				ImGui::PopID();
			}

			ImGui::EndTable();
		}
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding Size", &padding, 0, 32);
		ImGui::EndTable();
	}
	//if(op)
	//	ImGui::OpenPopup("Warning##openfile");
	//if (ImGui::BeginPopupModal("Warning##openfile"))
	//{
	//	
	//	ImGui::Text("Opening file");
	//	if (ImGui::Button("Close"))
	//	{
	//		ImGui::CloseCurrentPopup();

	//	}
	//	ImGui::EndPopup();
	//}
	
	ImGui::End();
}

void AssetsPanel::DisplayNode(const std::filesystem::directory_entry& directoryEntry)
{
	const auto& path = directoryEntry.path();
	
	//ImGui::Text("%s", path.string().c_str());
	//auto relativePath = std::filesystem::relative(directoryEntry.path(), s_EditorPath);
	std::string filenameString = path.filename().string();
	
	if (directoryEntry.is_directory())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (!std::filesystem::is_empty(path))
		{
			char str[25];
			sprintf_s(str, 25, "##%s", filenameString.c_str());
			bool open = ImGui::TreeNodeEx(str);
			ImGui::SameLine();
			if (open)
			{
				for (auto& p : std::filesystem::directory_iterator(path))
				{
					DisplayNode(p);
				}
				ImGui::TreePop();
			}
		}

		if (ImGui::Button(filenameString.c_str()))
		{
			m_CurrentPath = path;
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
	}
}
