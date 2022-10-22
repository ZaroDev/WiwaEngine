#include "InspectorPanel.h"

#include <imgui.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/scene/Scene.h>

#include <imgui_internal.h>
#include <Wiwa/Application.h>
#include <Wiwa/Resources.h>
#include <Wiwa/utilities/render/Material.h>

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

	if (ImGui::CollapsingHeader(name.c_str()))
	{
		byte* data = m_EntityManager->GetComponent(m_CurrentID, componentId, cl->size);
		if (strcmp(name.c_str(), "Mesh") == 0)
		{
			/*Wiwa::Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(((ResourceId)data + cl->fields[1].offset));
			ImGui::Image((ImTextureID)mat->getTextureId(), {64, 64});*/
			return;
		}

		for (size_t i = 0; i < cl->fields.size(); i++)
		{
			DrawField(data, cl->fields[i]);
		}
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
		DrawInt2Control(field.name, data, field);
	}
	else if (std::strcmp(field.type->name, "float") == 0)
	{
		ImGui::InputFloat("", (float*)(data + field.offset));
	}
	else if (std::strcmp(field.type->name, "struct Wiwa::Vector2f") == 0)
	{
		DrawVec2Control(field.name, data, field);
	}
	else if (std::strcmp(field.type->name, "struct Wiwa::Vector3f") == 0)
	{
		DrawVec3Control("", data, field);
	}
	else if (std::strcmp(field.type->name, "unsigned __int64") == 0)
	{
		ImGui::InputInt("", (int*)(data + field.offset));
	}
	else if (std::strcmp(field.type->name, "int") == 0) 
	{
		ImGui::InputInt("", (int*)(data + field.offset));
	}
	else if (std::strcmp(field.type->name, "struct Wiwa::Rect2i") == 0)
	{
		DrawRect2Control(field.name, data, field);
	}
	else if (std::strcmp(field.type->name, "enum Wiwa::Renderer2D::Pivot") == 0)
	{
		ImGui::InputInt("", (int*)(data + field.offset));
	}

	ImGui::PopID();
}

void InspectorPanel::DrawVec3Control(const char* label, unsigned char* data, const Field field, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label);

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label);
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		*(float*)(data + field.offset) = resetValue;
	}

	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", (float*)(data + field.offset), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		*(float*)(data + field.offset + (sizeof(float))) = resetValue;
	}

	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", (float*)(data + field.offset + (sizeof(float))), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		*(float*)(data + field.offset + (sizeof(float) * 2)) = resetValue;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", (float*)(data + field.offset + (sizeof(float) * 2)), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void InspectorPanel::DrawVec2Control(const char* label, unsigned char* data, const Field field, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label);

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label);
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		*(float*)(data + field.offset) = resetValue;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", (float*)(data + field.offset), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		*(float*)(data + field.offset + (sizeof(float))) = resetValue;
	}

	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", (float*)(data + field.offset +(sizeof(float))), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();


	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void InspectorPanel::DrawInt2Control(const char* label, unsigned char* data, const Field field, int resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label);

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label);
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		*(int*)(data + field.offset) = resetValue;
	}

	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragInt("##X", (int*)(data + field.offset), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		*(int*)(data + field.offset + (sizeof(float))) = resetValue;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragInt("##Y", (int*)(data + field.offset + (sizeof(float))), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void InspectorPanel::DrawRect2Control(const char* label, unsigned char* data, const Field field, int resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label);

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label);
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		*(int*)(data + field.offset) = resetValue;
	}

	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragInt("##X", (int*)(data + field.offset), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		*(int*)(data + field.offset + (sizeof(float))) = resetValue;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragInt("##Y", (int*)(data + field.offset + (sizeof(float))), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("W", buttonSize))
	{
		*(int*)(data + field.offset + (sizeof(float)) * 2) = resetValue;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragInt("##Width", (int*)(data + field.offset + (sizeof(float) * 2)), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("H", buttonSize))
	{
		*(int*)(data + field.offset + (sizeof(float)) * 3) = resetValue;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragInt("##H", (int*)(data + field.offset + (sizeof(float) * 3)), 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();


	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

InspectorPanel::InspectorPanel()
	: Panel("Inspector")
{
	m_EntityManager = &Wiwa::Application::Get().GetEntityManager();
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

		size_t tcount = Wiwa::Application::Get().getCoreTypeCount();
		size_t tacount = Wiwa::Application::Get().getAppTypeCount();

		if (ImGui::BeginPopup("Components"))
		{
			static ImGuiTextFilter filter;
			ImGui::Text("Search:");
			filter.Draw("##searchbar", 340.f);
			ImGui::BeginChild("listbox child", ImVec2(300, 200));
			// Core components
			for (size_t i = 0; i < tcount; i++) {
				const Type* type = Wiwa::Application::Get().getCoreType(i);
				const char* paintkit = type->name;
				if (filter.PassFilter(paintkit)) 
				{
					std::string label = paintkit;
					ClearComponentName(label);
					label += "##" + std::to_string(i);
					if (ImGui::MenuItem(label.c_str()))
					{
						entityManager.AddComponent(m_CurrentID, type->hash);
						ImGui::CloseCurrentPopup();
					}
				}
			}
			// App components
			for (size_t i = 0; i < tacount; i++) {
				const Type* type = Wiwa::Application::Get().getAppType(i);
				const char* paintkit = type->name;
				if (filter.PassFilter(paintkit))
				{
					std::string label = paintkit;
					ClearComponentName(label);
					label += "##" + std::to_string(i);
					if (ImGui::MenuItem(label.c_str()))
					{
						entityManager.AddComponent(m_CurrentID, type->hash);
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
