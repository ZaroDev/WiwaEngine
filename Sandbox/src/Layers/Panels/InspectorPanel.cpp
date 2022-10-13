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

void InspectorPanel::ClearComponentName(std::string& cname)
{
	// Clear namespace
	size_t nspace = cname.find(':');

	if (nspace != cname.npos) {
		cname.erase(0, nspace + 2);
		return;
	}

	// Clear class
	size_t ptype = cname.find("class ");

	if (ptype != cname.npos) {
		cname.erase(0, 6);
		return;
	}

	// Clear struct
	ptype = cname.find("struct ");

	if (ptype != cname.npos) {
		cname.erase(0, 7);
		return;
	}

	// Clear enum
	ptype = cname.find("enum ");

	if (ptype != cname.npos) {
		cname.erase(0, 5);
		return;
	}
}

void InspectorPanel::DrawComponent(size_t componentId)
{
	const Type* type = m_EntityManager->GetComponentType(componentId);
	const Class* cl = (const Class*)type;

	std::string name = cl->name;
	ClearComponentName(name);

	if (ImGui::TreeNodeEx(name.c_str()))
	{
		byte* data = m_EntityManager->GetComponent(m_CurrentID, componentId, cl->size);
		for (size_t i = 0; i < cl->fields.size(); i++)
		{
			DrawField(data, cl->fields[i]);
		}
		ImGui::TreePop();
	}
}

void InspectorPanel::DrawField(unsigned char* data, const Field& field)
{
	// Draw class field
	if (field.type->is_class) {
		const Class* cl = (const Class*)field.type;

		std::string name = field.name;
		ClearComponentName(name);

		if (ImGui::TreeNodeEx(name.c_str()))
		{
			for (size_t i = 0; i < cl->fields.size(); i++)
			{
				DrawField(data + field.offset, cl->fields[i]);
			}
			ImGui::TreePop();
		}
		return;
	}

	// Draw enum field
	if (field.type->is_enum) {
		const Enum* en = (const Enum*)field.type;

		ImGui::Text(field.name);
		ImGui::PushID(field.name);

		//TODO: DRAW LIST OF ENUMS TO CHOOSE FROM

		ImGui::PopID();
		return;
	}

	// Draw basic fields
	ImGui::Text(field.name);
	ImGui::PushID(field.name);
	if (std::strcmp(field.type->name, "struct Wiwa::Vector2i") == 0)
	{
		ImGui::InputInt("x", (int*)(data + field.offset));
		ImGui::InputInt("y", (int*)(data + field.offset + sizeof(int)));
	}
	else if (std::strcmp(field.type->name, "float") == 0)
	{
		ImGui::InputFloat("", (float*)(data + field.offset));
	}
	else if (std::strcmp(field.type->name, "struct Wiwa::Vector2f") == 0)
	{
		ImGui::InputFloat("x", (float*)(data + field.offset));
		ImGui::InputFloat("y", (float*)(data + field.offset + sizeof(float)));
	}
	else if (std::strcmp(field.type->name, "struct Wiwa::Vector3f") == 0)
	{
		ImGui::InputFloat("x", (float*)(data + field.offset));
		ImGui::InputFloat("y", (float*)(data + field.offset + sizeof(float)));
		ImGui::InputFloat("z", (float*)(data + field.offset + (sizeof(float) * 2)));
	}
	else if (std::strcmp(field.type->name, "unsigned __int64") == 0)
	{
		ImGui::InputInt("", (int*)(data + field.offset));
	}
	else if (std::strcmp(field.type->name, "struct Wiwa::Rect2i") == 0)
	{
		ImGui::InputInt("x", (int*)(data + field.offset));
		ImGui::InputInt("y", (int*)(data + field.offset + sizeof(int)));
		ImGui::InputInt("width", (int*)(data + field.offset + sizeof(int) * 2));
		ImGui::InputInt("height", (int*)(data + field.offset + sizeof(int) * 3));
	}
	else if (std::strcmp(field.type->name, "enum Wiwa::Renderer2D::Pivot") == 0)
	{
		ImGui::InputInt("", (int*)(data + field.offset));
	}

	ImGui::PopID();
}

InspectorPanel::InspectorPanel()
	: Panel("Inspector")
{
	m_EntityManager = &Wiwa::Application::Get().GetEntityManager();

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
		const char* entName = entityManager.GetEntityName(m_CurrentID);
		ImGui::Text(entName);
		std::map<ComponentId, size_t>& map = entityManager.GetEntityComponents(m_CurrentID);
		for (std::map<ComponentId, size_t>::iterator comp = map.begin(); comp != map.end(); comp++)
		{
			DrawComponent(comp->first);
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
