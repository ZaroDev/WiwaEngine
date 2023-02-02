#include "ProjectPanel.h"

#include <imgui.h>

#include <Wiwa/core/Application.h>

ProjectPanel::ProjectPanel(EditorLayer* instance) 
	: Panel("Project", ICON_FK_DATABASE, instance)
{
}

ProjectPanel::~ProjectPanel()
{
}

void ProjectPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active, ImGuiWindowFlags_NoDocking);
	const char* buff = Wiwa::Application::Get().GetProjectName();
	std::string edit = buff;

	ImGui::InputText("Project Name", (char*)edit.c_str(), 64);
	if (strcmp(edit.c_str(), buff) != 0)
		Wiwa::Application::Get().SetProjectName(edit.c_str());

	buff = Wiwa::Application::Get().GetProjectVersion();
	edit = buff;

	ImGui::InputText("Project Version", (char*)edit.c_str(), 64);
	if (strcmp(edit.c_str(), buff) != 0)
		Wiwa::Application::Get().SetProjectVersion(edit.c_str());

	buff = Wiwa::Application::Get().GetProjectCompany();
	edit = buff;

	ImGui::InputText("Company Name", (char*)edit.c_str(), 64);
	if (strcmp(edit.c_str(), buff) != 0)
		Wiwa::Application::Get().SetProjectCompany(edit.c_str());

	ImGui::Separator();
	const char* types[] = { "Windows", "We don't support more platforms you fool" };
	static const char* currentItem =
		Wiwa::Application::Get().GetProjectTarget() == Wiwa::ProjectTarget::Windows ? types[0] : types[1];
	if (ImGui::BeginCombo("Target", currentItem))
	{
		for (int i = 0; i < 2; i++)
		{
			bool isSelected = (currentItem == types[i]);
			if (ImGui::Selectable(types[i], isSelected))
			{
				currentItem = types[i];
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::Separator();
	static const char* item_names[] = { "Scene 1", "Scene 2", "Scene 3", "Scene 4", "Scene 5" };
	for (int n = 0; n < IM_ARRAYSIZE(item_names); n++)
	{
		const char* item = item_names[n];
		ImGui::Selectable(item);

		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (n_next >= 0 && n_next < IM_ARRAYSIZE(item_names))
			{
				item_names[n] = item_names[n_next];
				item_names[n_next] = item;
				ImGui::ResetMouseDragDelta();
			}
		}
	}

	ImGui::End();
}
