#include "AssetsPanel.h"

#include <imgui.h>
#include "wipch.h"


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

	if (m_CurrentPath != std::filesystem::path(s_AssetsPath))
	{
		if (ImGui::Button("<-"))
		{
			m_CurrentPath = m_CurrentPath.parent_path();
		}
	}
	
	static float padding = 16.0f;
	static float thumbnailSize = 64.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	int id = 0;

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentPath))
	{
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
			}

		}
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::Columns(1);
	ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	ImGui::SliderFloat("Padding Size", &padding, 0, 32);

	ImGui::End();
}
