#include "wipch.h"
#include "ResourcesPanel.h"
#include <imgui.h>
#include <Wiwa/core/Resources.h>

ResourcesPanel::ResourcesPanel(EditorLayer* instance) 
	: Panel("Resources", ICON_FK_FILE, instance)
{
}

ResourcesPanel::~ResourcesPanel()
{
}

void ResourcesPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
	ImGui::Text("Resources Loaded");
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Images"))
	{
		std::vector<Wiwa::Resources::Resource*>& images = Wiwa::Resources::GetResourcesOf(Wiwa::Resources::WRT_IMAGE);
		size_t count = images.size();
		for (size_t i = 0; i < count; i++)
		{
			ImGui::Text(images.at(i)->filePath.c_str());
		}
		ImGui::Text("%i bytes", (sizeof(Wiwa::Image) * images.size()));
	}

	if (ImGui::CollapsingHeader("Models"))
	{
		std::vector<Wiwa::Resources::Resource*>& models = Wiwa::Resources::GetResourcesOf(Wiwa::Resources::WRT_MODEL);
		size_t count = models.size();
		for (size_t i = 0; i < count; i++)
		{
			ImGui::Text(models.at(i)->filePath.c_str());
		}
		ImGui::Text("%i bytes", (sizeof(Wiwa::Model) * models.size()));
	}

	if (ImGui::CollapsingHeader("Shaders"))
	{
		std::vector<Wiwa::Resources::Resource*>& shaders = Wiwa::Resources::GetResourcesOf(Wiwa::Resources::WRT_SHADER);
		size_t count = shaders.size();
		for (size_t i = 0; i < count; i++)
		{
			ImGui::Text(shaders.at(i)->filePath.c_str());
		}
		ImGui::Text("%i bytes", (sizeof(Wiwa::Shader) * shaders.size()));
	}


	if (ImGui::CollapsingHeader("Audio clips"))
	{
		std::vector<Wiwa::Resources::Resource*>& audios = Wiwa::Resources::GetResourcesOf(Wiwa::Resources::WRT_AUDIOCLIP);
		size_t count = audios.size();
		for (size_t i = 0; i < count; i++)
		{
			ImGui::Text(audios.at(i)->filePath.c_str());
		}
	}
	if (ImGui::CollapsingHeader("Materials"))
	{
		std::vector<Wiwa::Resources::Resource*>& materials = Wiwa::Resources::GetResourcesOf(Wiwa::Resources::WRT_MATERIAL);
		size_t count = materials.size();
		for (size_t i = 0; i < count; i++)
		{
			ImGui::Text(materials.at(i)->filePath.c_str());
		}
		ImGui::Text("%i bytes", (sizeof(Wiwa::Material) * materials.size()));
	}
	ImGui::End();
}
