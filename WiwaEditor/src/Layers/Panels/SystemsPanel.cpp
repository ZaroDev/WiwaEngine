#include <wipch.h>
#include "SystemsPanel.h"
#include <unordered_map>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/scripting/ScriptEngine.h>
SystemsPanel::SystemsPanel(EditorLayer* instance) 
	: Panel("Systems", instance)
{
}


SystemsPanel::~SystemsPanel()
{
}

void SystemsPanel::Draw()
{
	ImGui::Begin(name, &active);
	ImGui::Text("Active Systems:");
	ImGui::Separator();
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	std::unordered_map<size_t, Type*> systems = Wiwa::ScriptEngine::getSystems();
	for (auto system = systems.begin(); system != systems.end(); system++)
	{
		ImGui::TreeNodeEx(system->second->name.c_str(), flags);
	}

	ImGui::End();
}
