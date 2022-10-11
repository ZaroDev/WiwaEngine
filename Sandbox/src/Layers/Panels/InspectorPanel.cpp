#include "InspectorPanel.h"

#include <imgui.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/Application.h>

uint32_t InspectorPanel::m_CurrentID = 0;
bool InspectorPanel::m_EntitySet = false;

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
		std::map<ComponentId, size_t>& map = entityManager.GetEntityComponents(m_CurrentID);
		for (std::map<ComponentId, size_t>::iterator comp = map.begin(); comp != map.end(); comp++)
		{
			const Type* type = entityManager.GetComponentType(comp->first);
			const Class* cl = (const Class*)type;

			if (ImGui::TreeNodeEx(cl->name))
			{
				byte* data = entityManager.GetComponent(m_CurrentID, comp->first, cl->size);
				for (size_t i = 0; i < cl->fields.size(); i++)
				{
					ImGui::Text(cl->fields[i].name);

					if (std::strcmp(cl->fields[i].type->name, "float") == 0)
					{
						ImGui::InputFloat(cl->fields[i].name, (float*)data);
					}
					else if (std::strcmp(cl->fields[i].type->name, "struct Wiwa::Vector2f") == 0)
					{
						ImGui::InputFloat2(cl->fields[i].name, (float*)data);
					}
					else if (std::strcmp(cl->fields[i].type->name, "struct Wiwa::Vector2i") == 0)
					{
						ImGui::InputInt2(cl->fields[i].name, (int*)data);
					}
				}
				ImGui::TreePop();
			}
		}

		if (ButtonCenteredOnLine("Add component"))
		{
			entityManager.AddComponent<Wiwa::Transform3D>(m_CurrentID);
		}
	}
	ImGui::End();
}

void InspectorPanel::Update()
{
}
