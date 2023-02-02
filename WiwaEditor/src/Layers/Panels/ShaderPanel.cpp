#include "wipch.h"
#include "ShaderPanel.h"
#include "../../Utils/EditorUtils.h"
#include <Wiwa/core/Resources.h>


ShaderPanel::ShaderPanel(EditorLayer* instance)
	: Panel("Shaders",ICON_FK_MAP_O, instance)
{

}

ShaderPanel::~ShaderPanel()
{

}

void ShaderPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
	
	std::string str = m_Shader ? m_ShaderPath : "Shaders";
	if (ButtonCenteredOnLine(str.c_str()))
		ImGui::OpenPopup("Shaders");

	if (ImGui::BeginPopup("Shaders"))
	{
		static ImGuiTextFilter filter;
		ImGui::Text("Search:");
		filter.Draw("##searchbar", 340.f);
		ImGui::BeginChild("listbox child", ImVec2(300, 200));
		std::vector<Wiwa::Resources::Resource*>& shaders = Wiwa::Resources::GetResourcesOf(Wiwa::Resources::WRT_SHADER);
		for (size_t i = 0; i < shaders.size(); i++)
		{
			if (filter.PassFilter(shaders[i]->filePath.c_str()))
			{
				std::string label = shaders[i]->filePath.c_str();

				label += "##" + std::to_string(i);
				if (ImGui::MenuItem(label.c_str()))
				{
					size_t id = Wiwa::Resources::Load<Wiwa::Shader>(shaders[i]->filePath.c_str());
					m_Shader = Wiwa::Resources::GetResourceById<Wiwa::Shader>(id);
					m_ShaderPath = shaders[i]->filePath;
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndPopup();
	}

	if (m_Shader)
	{
		ImGui::Text("Status: ");
		ImGui::SameLine();
		m_Shader->getState() == Wiwa::Shader::ToCompile ?
			ImGui::TextColored(ImVec4(255, 252, 127, 1), "To compile") :
			m_Shader->getState() == Wiwa::Shader::Error ?
			ImGui::TextColored(ImVec4(255, 0, 0, 1), "Error") : ImGui::TextColored(ImVec4(0, 255, 0, 1), "Compiled");

	

		if (ImGui::CollapsingHeader("Refs"))
		{
			std::vector<Wiwa::Material*>& refs = m_Shader->getRefs();
			ImGui::Text("%i", refs.size());
			for (Wiwa::Material* mat : refs)
			{
				ImGui::Text(mat->getMaterialPath());
				ImGui::Separator();
			}
		}
		if (ImGui::Button("Add uniform"))
			ImGui::OpenPopup("Types");
		if (ImGui::BeginPopup("Types"))
		{
			static char uName[64];
			ImGui::InputText("Name", uName, 64);

			const char* types[] =
			{
				"Bool",
				"Int",
				"Uint",
				"Float",
				"Vec2",
				"Vec3",
				"Color",
				"Mat2",
				"Mat3",
				"Mat4",
				"Time",
				"Sampler2D"
			};
			static const char* currentItem = types[0];
			static Wiwa::UniformType type;
			if (ImGui::BeginCombo("Type", currentItem))
			{
				for (int i = 0; i < 12; i++)
				{
					bool isSelected = (currentItem == types[i]);
					if (ImGui::Selectable(types[i], isSelected))
					{
						currentItem = types[i];
						type = (Wiwa::UniformType)(i);
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (ImGui::Button("Add"))
			{
				m_Shader->addUniform(uName, type);
			}
			ImGui::EndPopup();
		}
		std::vector<Wiwa::UniformField>& fields = m_Shader->getUniforms();
		ImGui::Separator();
		int id = 0;
		const char* nameToDelete = nullptr;
		for (Wiwa::UniformField& field : fields)
		{	
			ImGui::PushID(id++);
			DrawField(&field, nameToDelete);
			ImGui::PopID();
		}
		
		if (ImGui::Button("Compile"))
		{
			m_Shader->Save();
			m_Shader->CompileWishader(m_ShaderPath.c_str());
		}

		if (nameToDelete)
		{
			m_Shader->deleteUniform(nameToDelete);
		}
	}
	ImGui::End();

}

void ShaderPanel::DrawField(Wiwa::UniformField* field, const char*&nameToDelete)
{
	static char uName[64];
	memcpy(uName, field->name.c_str(), field->name.size() + 1);

	ImGui::InputText("Name", uName, 64);

	const char* types[] =
	{
		"Bool",
		"Int",
		"Uint",
		"Float",
		"Vec2",
		"Vec3",
		"Color",
		"Mat2",
		"Mat3",
		"Mat4",
		"Time",
		"Sampler2D"
	};
	const char* currentItem = types[(int)field->type];
	static Wiwa::UniformType type = field->type;
	if (ImGui::BeginCombo("Type", currentItem))
	{
		for (int i = 0; i < 12; i++)
		{
			bool isSelected = (currentItem == types[i]);
			if (ImGui::Selectable(types[i], isSelected))
			{
				currentItem = types[i];
				type = (Wiwa::UniformType)(i);
				m_Shader->setUniformType(field->name.c_str(), type);
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	m_Shader->setUniformName(field->name.c_str(), uName);
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	if (ImGui::Button("Delete"))
	{
		nameToDelete = uName;
	}
	ImGui::PopStyleColor(3);
	ImGui::Separator();
}
