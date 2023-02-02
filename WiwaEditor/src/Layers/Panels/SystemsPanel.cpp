#include <wipch.h>
#include "SystemsPanel.h"
#include <unordered_map>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/scripting/ScriptEngine.h>
#include <Wiwa/core/Application.h>

SystemsPanel::SystemsPanel(EditorLayer* instance) 
	: Panel("Systems", ICON_FK_FILTER, instance)
{

}

SystemsPanel::~SystemsPanel()
{

}

void SystemsPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
	ImGui::Text("Active Systems:");
	ImGui::Separator();
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	
	Wiwa::Application& app = Wiwa::Application::Get();

	size_t system_count = app.GetSystemTypeCount();

	for (size_t i=0;i<system_count;i++)
	{
		const Type* s_type = app.GetSystemType(i);

		ImGui::TreeNodeEx(s_type->name.c_str(), flags);
	}

	ImGui::End();
}
