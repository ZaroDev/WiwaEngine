#include <wipch.h>
#include "MaterialPanel.h"

#include <Wiwa/core/Resources.h>

#include <Wiwa/utilities/json/JSONDocument.h>

#include "../../Utils/EditorUtils.h"

#include <Wiwa/core/Renderer3D.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/utilities/render/shaders/Uniform.h>


MaterialPanel::MaterialPanel(EditorLayer* instance)
	: Panel("Material Editor", ICON_FK_CIRCLE_O, instance)
{
}

MaterialPanel::~MaterialPanel()
{
}

void MaterialPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

    
    if (m_Material)
    {
		ImGui::Text(m_Material->getMaterialPath());
		std::string str = m_Material->getShader()->getPath();
		if (ButtonCenteredOnLine("Shaders"))
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
						m_Material->setShader(Wiwa::Resources::GetResourceById<Wiwa::Shader>(id), shaders[i]->filePath.c_str());
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::EndChild();
			ImGui::EndPopup();
		}
        std::vector<Wiwa::Uniform>& uniforms = m_Material->getUniforms();
        for (size_t i = 0; i < uniforms.size(); i++)
        {
            RenderUniform(&uniforms[i]);
        }
		if (ImGui::Button("Refresh"))
			m_Material->Refresh();

		ImGui::SameLine();
        if (ImGui::Button("Save"))
            m_Material->Save(s_Path.string().c_str());

    }
    else
       TextCentered("Select a material to begin editing");


   
	ImGui::End();

	/* ImGui::Begin("Light Debugger");
	 Wiwa::DirectionalLight& directionalLight = Wiwa::Application::Get().GetRenderer3D().getFrameBuffer()->getDirectionalLight();
	 ImGui::DragFloat3("Direction", glm::value_ptr(directionalLight.Direction));
	 ImGui::ColorEdit3("Ambient", glm::value_ptr(directionalLight.Ambient));
	 ImGui::ColorEdit3("Diffuse", glm::value_ptr(directionalLight.Diffuse));
	 ImGui::ColorEdit3("Specular", glm::value_ptr(directionalLight.Specular));
	 Wiwa::Application::Get().GetRenderer3D().getFrameBuffer()->setLight(directionalLight);*/

    //Wiwa::List<Wiwa::PointLight>* pLights = Wiwa::Application::Get().GetRenderer3D().getFrameBuffer().getPointLights();
    /*if (ImGui::Button("+"))
    {
        Wiwa::PointLight light{
            glm::vec3{0.0f},
            glm::vec3{1.0f},
            glm::vec3{1.0f},
            glm::vec3{1.0f},
            1.0f,
            0.09f,
            0.032f
        };
        Wiwa::Application::Get().GetRenderer3D().getFrameBuffer().addPointLight(light);
    }
    for (int i = 0; i < pLights->size(); i++)
    {
        ImGui::PushID(i);
        ImGui::DragFloat3("Position", glm::value_ptr(pLights->at(i).Position));
        ImGui::ColorEdit3("Ambient", glm::value_ptr(pLights->at(i).Ambient));
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(pLights->at(i).Diffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(pLights->at(i).Specular));
        ImGui::DragFloat("Constant", &pLights->at(i).Constant);
        ImGui::DragFloat("Linear", &pLights->at(i).Linear);
        ImGui::DragFloat("Quadratic", &pLights->at(i).Quadratic);
        ImGui::PopID();
    }*/


    //ImGui::End();
}



void MaterialPanel::OnEvent(Wiwa::Event& e)
{
    Wiwa::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MaterialChangeEvent>({ &MaterialPanel::OnMaterialChange, this });

}

bool MaterialPanel::OnMaterialChange(MaterialChangeEvent& e)
{
    m_Material = Wiwa::Resources::GetResourceById<Wiwa::Material>(e.GetResourceId());
	s_Path = m_Material->getMaterialPath();
    return true;
}

void MaterialPanel::RenderUniform(Wiwa::Uniform* uniform)
{
	switch (uniform->getType())
	{
	case Wiwa::UniformType::Bool:
		ImGui::Checkbox(uniform->name.c_str(), uniform->getPtrData<bool>());
		break;
	case Wiwa::UniformType::Int:
		ImGui::DragInt(uniform->name.c_str(), uniform->getPtrData<int>());
		break;
	case Wiwa::UniformType::Uint:
		ImGui::DragInt(uniform->name.c_str(), uniform->getPtrData<int>());
		break;
	case Wiwa::UniformType::Float:
		ImGui::DragFloat(uniform->name.c_str(), uniform->getPtrData<float>());
		break;
	case Wiwa::UniformType::fVec2:
		ImGui::DragFloat2(uniform->name.c_str(), uniform->getPtrData<float>());
		break;
	case Wiwa::UniformType::fVec3:
		ImGui::DragFloat3(uniform->name.c_str(), uniform->getPtrData<float>());
		break;
	case Wiwa::UniformType::fVec4:
		ImGui::ColorEdit4(uniform->name.c_str(), uniform->getPtrData<float>());
		break;
	case Wiwa::UniformType::Mat2:
		break;
	case Wiwa::UniformType::Mat3:
		break;
	case Wiwa::UniformType::Mat4:

		break;
	case Wiwa::UniformType::Time:
	{
		ImGui::Text(uniform->name.c_str());
		ImGui::SameLine();
		ImGui::Text("%f", uniform->getPtrData<float>());
	}break;
	case Wiwa::UniformType::Sampler2D:
	{
		ImGui::Text(uniform->name.c_str());
		ImGui::SameLine();
		ImGui::Image((ImTextureID)(intptr_t)uniform->getPtrData<glm::ivec2>()->x, { 64, 64 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::wstring ws(path);
				std::string pathS(ws.begin(), ws.end());
				std::filesystem::path p = pathS;
				if (m_Material)
				{
					if (ImageExtensionComp(p)) 
					{
						uint32_t id = Wiwa::Resources::Load<Wiwa::Image>(p.string().c_str());
						Wiwa::Image* img = Wiwa::Resources::GetResourceById<Wiwa::Image>(id);
						uniform->setData(glm::ivec2(img->GetTextureId(), id), uniform->getType());
					}
				}

			}
			ImGui::EndDragDropTarget();

		}
	}break;
	default:
		break;
	}
}
