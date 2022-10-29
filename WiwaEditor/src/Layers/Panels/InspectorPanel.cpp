#include "InspectorPanel.h"

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/scene/Scene.h>

#include <imgui_internal.h>
#include <Wiwa/Application.h>
#include <Wiwa/Resources.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/math/Vector2i.h>
#include "../../Utils/ImGuiWidgets.h"

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

	//Draw custom fields
	if (strcmp(field.name, "materialId") == 0) 
	{
		ImGui::Text("Material");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::wstring ws(path);
				std::string pathS(ws.begin(), ws.end());
				std::filesystem::path p = pathS.c_str();
				if (p.extension() == ".wimaterial")
				{
					WI_INFO("Trying to load payload at path {0}", pathS.c_str());
					ResourceId id = Wiwa::Resources::Load<Wiwa::Material>(pathS.c_str());
					*(ResourceId*)(data + field.offset) = id;
				}
			}

			ImGui::EndDragDropTarget();
		}
		ImGui::PushID(field.name);
		int id = *(int*)(data + field.offset);
		Wiwa::Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(id);
		ImGui::Text("Material at: ");
		ImGui::SameLine();
		ImGui::Text(mat->getMaterialPath());
		ImGui::Image((ImTextureID)(intptr_t)mat->getTextureId(), { 64, 64 });
		ImGui::Text("Texture size: ");
		ImGui::SameLine();
		ImGui::Text("%i", mat->getTextureSize().x);
		ImGui::SameLine();
		ImGui::Text("x %i", mat->getTextureSize().y);
		const char* types[] = { "Color", "Textured" };
		const char* currentItem = mat->getType() == Wiwa::Material::MaterialType::color ? types[0] : types[1];
		ImGui::Text("Texture path: %s", mat->getTexturePath());
		ImGui::Text("Type");
		
		ImGui::SameLine();
		ImGui::Text(currentItem);
		static glm::vec4 color = { mat->getColor().r, mat->getColor().g,mat->getColor().b , mat->getColor().a };
		ImGui::ColorEdit4("Color", glm::value_ptr(color));

		Wiwa::Material::MaterialSettings& settings = mat->getSettings();
		ImGui::ColorEdit3("Diffuse", glm::value_ptr(settings.diffuse));
		ImGui::ColorEdit3("Specular", glm::value_ptr(settings.specular));
		ImGui::DragFloat("Shininess", &settings.shininess, 0.1f, 0, 1);
		ImGui::Text(mat->getTexturePath());
		static bool checker = false;
		if(ImGui::Checkbox("Set Checker", &checker))
		{
			mat->setTexture("checker");
			if (!checker)
			{
				mat->setTexture(mat->getTexturePath());
			}
		}

		const char* type = mat->getType() == 0 ? "Type: Color" : "Type: Texture";
		ImGui::Text(type);
		ImGui::PopID();
		return;
	}
	if (strcmp(field.name, "meshId") == 0)
	{
		ImGui::Text("Model");
		ImGui::PushID(field.name);
		int id = *(int*)(data + field.offset);
		Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(id);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::wstring ws(path);
				std::string pathS(ws.begin(), ws.end());
				std::filesystem::path p = pathS.c_str();
				if (p.extension() == ".fbx" || p.extension() == ".FBX")
				{
					WI_INFO("Trying to load payload at path {0}", pathS.c_str());
					ResourceId id = Wiwa::Resources::Load<Wiwa::Model>(pathS.c_str());
					*(ResourceId*)(data + field.offset) = id;
				}
			}

			ImGui::EndDragDropTarget();
		}
		ImGui::Text("Model path: ");
		ImGui::SameLine();
		ImGui::Text(mod->getModelPath());
		static bool showNormals = false;
		if (ImGui::Checkbox("Show normals", &showNormals))
			mod->showNormals = showNormals;
		ImGui::PopID();
		return;
	}
	// Draw basic fields
	if (std::strcmp(field.type->name, "struct Wiwa::Vector2i") == 0)
	{
		ImGui::PushID(field.name);
		DrawInt2Control(field.name, data, field);
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name, "float") == 0)
	{
		ImGui::Text(field.name);
		ImGui::PushID(field.name);
		ImGui::InputFloat("", (float*)(data + field.offset));
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name, "struct Wiwa::Vector2f") == 0)
	{
		ImGui::PushID(field.name);
		DrawVec2Control(field.name, data, field);
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name, "struct Wiwa::Vector3f") == 0)
	{
		ImGui::PushID(field.name);
		if (std::strcmp(field.name, "scale") == 0)
			DrawVec3Control(field.name, data, field, 1.0);
		else
			DrawVec3Control(field.name, data, field,0.0f);

		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name, "unsigned __int64") == 0)
	{
		ImGui::Text(field.name);
		ImGui::PushID(field.name);
		ImGui::InputInt("", (int*)(data + field.offset));
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name, "int") == 0) 
	{
		ImGui::Text(field.name);
		ImGui::PushID(field.name);
		ImGui::InputInt("", (int*)(data + field.offset));
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name, "struct Wiwa::Rect2i") == 0)
	{
		ImGui::PushID(field.name);
		DrawRect2Control(field.name, data, field);
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name, "enum Wiwa::Renderer2D::Pivot") == 0)
	{
		ImGui::Text(field.name);
		ImGui::PushID(field.name);
		ImGui::InputInt("", (int*)(data + field.offset));
		ImGui::PopID();
	}

	
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
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 10 });

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
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 10 });

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
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 10 });

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
	ImGui::DragInt("##X", (int*)(data + field.offset));
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
	ImGui::DragInt("##Y", (int*)(data + field.offset + (sizeof(float))));
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
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 10 });

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
	ImGui::DragInt("##X", (int*)(data + field.offset));
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
	ImGui::DragInt("##Y", (int*)(data + field.offset + (sizeof(float))));
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
	ImGui::DragInt("##Width", (int*)(data + field.offset + (sizeof(float) * 2)));
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
	ImGui::DragInt("##H", (int*)(data + field.offset + (sizeof(float) * 3)));
	ImGui::PopItemWidth();


	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

InspectorPanel::InspectorPanel(EditorLayer* instance)
	: Panel("Inspector", instance)
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
	else
	{
		TextCentered("Select an entity to inspect");
	}
	ImGui::End();
}

void InspectorPanel::Update()
{
}

void InspectorPanel::OnEvent(Wiwa::Event&e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<EntityChangeEvent>({ &InspectorPanel::OnEntityChangeEvent, this });
}

bool InspectorPanel::OnEntityChangeEvent(EntityChangeEvent& e)
{
	m_CurrentID = e.GetResourceId();
	m_EntitySet = true;
	return false;
}
