#include "InspectorPanel.h"

#include <imgui.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/Application.h>

uint32_t InspectorPanel::m_CurrentID = 0;
bool InspectorPanel::m_EntitySet = false;

void RemoveWordFromLine(std::string& line, const std::string& word)
{
	auto n = line.find(word);
	if (n != std::string::npos)
	{
		line.erase(n, word.length());
	}
}

bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return ImGui::Button(label);
}

InspectorPanel::InspectorPanel()
	: Panel("Inspector")
{
	m_Components = GetTypes<1>();
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::Draw()
{
	Wiwa::EntityManager& entityManager = Wiwa::Application::Get().GetEntityManager();
	ImGui::Begin(name, &active);
	if (m_EntitySet)
	{
		int id = 0;
		const char* entName = entityManager.GetEntityName(m_CurrentID);
		ImGui::Text(entName);
		std::map<ComponentId, size_t>& map = entityManager.GetEntityComponents(m_CurrentID);
		for (std::map<ComponentId, size_t>::iterator comp = map.begin(); comp != map.end(); comp++)
		{
			const Type* type = entityManager.GetComponentType(comp->first);
			const Class* cl = (const Class*)type;

			std::string name = cl->name;
			RemoveWordFromLine(name, "struct Wiwa::");

			if (ImGui::TreeNodeEx(name.c_str()))
			{
				byte* data = entityManager.GetComponent(m_CurrentID, comp->first, cl->size);
				for (size_t i = 0; i < cl->fields.size(); i++)
				{
					ImGui::Text(cl->fields[i].name);
					ImGui::PushID(id++);
					if (std::strcmp(cl->fields[i].type->name, "struct Wiwa::Vector2i") == 0)
					{
						ImGui::InputInt("x",		(int*)(data + cl->fields[i].offset));
						ImGui::InputInt("y",		(int*)(data + cl->fields[i].offset + sizeof(int)));
					}
					else if (std::strcmp(cl->fields[i].type->name, "float") == 0)
					{
						ImGui::InputFloat("",		(float*)(data + cl->fields[i].offset));
					}
					else if (std::strcmp(cl->fields[i].type->name, "struct Wiwa::Vector2f") == 0)
					{
						ImGui::InputFloat("x",		(float*)(data + cl->fields[i].offset));
						ImGui::InputFloat("y",		(float*)(data + cl->fields[i].offset + sizeof(float)));
					}
					else if (std::strcmp(cl->fields[i].type->name, "struct Wiwa::Vector3f") == 0)
					{
						ImGui::InputFloat("x",		(float*)(data + cl->fields[i].offset));
						ImGui::InputFloat("y",		(float*)(data + cl->fields[i].offset + sizeof(float)));
						ImGui::InputFloat("z",		(float*)(data + cl->fields[i].offset + (sizeof(float) * 2)));
					}
					else if (std::strcmp(cl->fields[i].type->name, "unsigned __int64") == 0)
					{
						ImGui::InputInt("", (int*)(data + cl->fields[i].offset));
					}
					else if (std::strcmp(cl->fields[i].type->name, "struct Wiwa::Rect2i") == 0)
					{
						ImGui::InputInt("x",		(int*)(data + cl->fields[i].offset));
						ImGui::InputInt("y",		(int*)(data + cl->fields[i].offset + sizeof(int)));
						ImGui::InputInt("width",	(int*)(data + cl->fields[i].offset + sizeof(int) * 2));
						ImGui::InputInt("height",	(int*)(data + cl->fields[i].offset + sizeof(int) * 3));
					}
					else if (std::strcmp(cl->fields[i].type->name, "enum Wiwa::Renderer2D::Pivot") == 0)
					{
						ImGui::InputInt("",			(int*)(data + cl->fields[i].offset));
					}

					ImGui::PopID();
				}
				ImGui::TreePop();
			}
		}

		if (ButtonCenteredOnLine("Add component"))
			ImGui::OpenPopup("Components");

		if (ImGui::BeginPopup("Components"))
		{
			static ImGuiTextFilter filter;
			ImGui::Text("Search:");
			filter.Draw("##searchbar", 340.f);
			ImGui::BeginChild("listbox child", ImVec2(300, 200));
			for (int i = 0; i < m_Components.Size(); i++) {
				const char* paintkit = m_Components[i]->name;
				if (filter.PassFilter(paintkit)) 
				{
					std::string label = paintkit;
					RemoveWordFromLine(label, "struct Wiwa::");
					label += "##" + std::to_string(i);
					if (ImGui::MenuItem(label.c_str()))
					{
						entityManager.AddComponent<Wiwa::Transform3D>(m_CurrentID);
						ImGui::CloseCurrentPopup();
					}
				}

			}
			ImGui::EndChild();
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void InspectorPanel::Update()
{
}
