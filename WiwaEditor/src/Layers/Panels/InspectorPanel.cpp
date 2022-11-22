#include "InspectorPanel.h"

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/scene/Scene.h>

#include <imgui_internal.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Resources.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/math/Vector2i.h>
#include "../../Utils/ImGuiWidgets.h"

void InspectorPanel::DrawComponent(size_t componentId)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	const Type* type = em.GetComponentType(componentId);

	std::string name = type->name;

	if (ImGui::CollapsingHeader(name.c_str()))
	{
		byte* data = em.GetComponent(m_CurrentID, componentId, type->size);

		if (type->is_class) {
			const Class* cl = (const Class*)type;

			for (size_t i = 0; i < cl->fields.size(); i++)
			{
				DrawField(data, cl->fields[i]);
			}
		}
	}
}

void InspectorPanel::DrawField(unsigned char* data, const Field& field)
{
	// Draw class field
	if (field.type->is_class) {
		const Class* cl = (const Class*)field.type;

		std::string name = field.name;

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

		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());

		//TODO: DRAW LIST OF ENUMS TO CHOOSE FROM

		ImGui::PopID();
		return;
	}

	//Draw custom fields
	if (strcmp(field.name.c_str(), "materialId") == 0)
	{
		ImGui::Text("Material");
		int id = *(int*)(data + field.offset);
		Wiwa::Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(id);
		AssetContainer(std::filesystem::path(mat->getMaterialPath()).stem().string().c_str());
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
		ImGui::PushID(field.name.c_str());

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
	if (strcmp(field.name.c_str(), "meshId") == 0)
	{
		ImGui::Text("Model");
		ImGui::PushID(field.name.c_str());
		int id = *(int*)(data + field.offset);
		Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(id);
		AssetContainer(std::filesystem::path(mod->getModelPath()).stem().string().c_str());
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
	if (std::strcmp(field.type->name.c_str(), "Vector2i") == 0)
	{
		ImGui::PushID(field.name.c_str());
		DrawInt2Control(field.name.c_str(), data, field);
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name.c_str(), "float") == 0)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputFloat("", (float*)(data + field.offset));
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name.c_str(), "Vector2f") == 0)
	{
		ImGui::PushID(field.name.c_str());
		DrawVec2Control(field.name.c_str(), data, field);
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name.c_str(), "Vector3f") == 0)
	{
		ImGui::PushID(field.name.c_str());
		if (std::strcmp(field.name.c_str(), "scale") == 0)
			DrawVec3Control(field.name.c_str(), data, field, 1.0);
		else
			DrawVec3Control(field.name.c_str(), data, field,0.0f);

		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name.c_str(), "unsigned __int64") == 0)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputInt("", (int*)(data + field.offset));
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name.c_str(), "int") == 0)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputInt("", (int*)(data + field.offset));
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name.c_str(), "Rect2i") == 0)
	{
		ImGui::PushID(field.name.c_str());
		DrawRect2Control(field.name.c_str(), data, field);
		ImGui::PopID();
	}
	else if (std::strcmp(field.type->name.c_str(), "Pivot") == 0)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputInt("", (int*)(data + field.offset));
		ImGui::PopID();
	}

	
}



InspectorPanel::InspectorPanel(EditorLayer* instance)
	: Panel("Inspector", instance)
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::Draw()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Begin(name, &active);
	if (m_EntitySet)
	{
		const char* entName = em.GetEntityName(m_CurrentID);
		std::string edit = entName;
		
		ImGui::InputText("Name", (char*)edit.c_str(), 64);
		if (ImGui::Button("Delete"))
		{
			m_EntitySet = false;
			em.DestroyEntity(m_CurrentID);
		}
		if (strcmp(edit.c_str(), entName) != 0)
			em.SetEntityName(m_CurrentID, edit.c_str());
		

		std::map<ComponentId, size_t>& map = em.GetEntityComponents(m_CurrentID);
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
				const char* paintkit = type->name.c_str();
				if (filter.PassFilter(paintkit)) 
				{
					std::string label = paintkit;

					label += "##" + std::to_string(i);
					if (ImGui::MenuItem(label.c_str()))
					{
						em.AddComponent(m_CurrentID, type);
						ImGui::CloseCurrentPopup();
					}
				}
			}
			// App components
			for (size_t i = 0; i < tacount; i++) {
				const Type* type = Wiwa::Application::Get().getAppType(i);
				const char* paintkit = type->name.c_str();
				if (filter.PassFilter(paintkit))
				{
					std::string label = paintkit;

					label += "##" + std::to_string(i);
					if (ImGui::MenuItem(label.c_str()))
					{
						em.AddComponent(m_CurrentID, type);
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
