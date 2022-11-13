#include <wipch.h>
#include "MaterialPanel.h"

#include <Wiwa/core/Resources.h>

#include <Wiwa/utilities/json/JSONDocument.h>

#include "../../Utils/ImGuiWidgets.h"

#include <Wiwa/core/Renderer3D.h>
#include <Wiwa/core/Application.h>


MaterialPanel::MaterialPanel(EditorLayer* instance)
	: Panel("Material Editor", instance)
{
}

MaterialPanel::~MaterialPanel()
{
}

void MaterialPanel::Draw()
{
	ImGui::Begin(name, &active);

    
    if (m_Material)
    {
        ImGui::Image((ImTextureID)(intptr_t)m_Material->getTextureId(), { 64, 64 });

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                std::wstring ws(path);
                std::string pathS(ws.begin(), ws.end());
                std::filesystem::path p = pathS;
                if (p.extension() == ".png" && m_Material)
                {  
                    m_Material->setTexture(pathS.c_str());
                }

            }
            ImGui::EndDragDropTarget();

        }
        ImGui::Text("Texture size: ");
        ImGui::SameLine();
        ImGui::Text("%i", m_Material->getTextureSize().x);
        ImGui::SameLine();
        ImGui::Text("x %i", m_Material->getTextureSize().y);
        ImGui::Text("Texture path: %s", m_Material->getTexturePath());
        glm::vec4 color = { m_Material->getColor().r, m_Material->getColor().g,m_Material->getColor().b , m_Material->getColor().a };
        ImGui::ColorEdit4("Color", glm::value_ptr(color));
        m_Material->setColor({ color.r, color.g, color.b, color.a });
        const char* types[] = { "Color", "Textured" };
        static const char* currentItem = m_Material->getType() == Wiwa::Material::MaterialType::color ? types[0] : types[1];
        
        Wiwa::Material::MaterialSettings& settings = m_Material->getSettings();
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(settings.diffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(settings.specular));
        ImGui::DragFloat("Shininess", &settings.shininess, 0.1f, 0, 1);
        m_Material->setSettings(settings);
        if (ImGui::BeginCombo("Material Type", currentItem))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = (currentItem == types[i]);
                if (ImGui::Selectable(types[i], isSelected))
                {
                    currentItem = types[i];
                    if (i == 0)
                    {
                        m_Material->setType(Wiwa::Material::MaterialType::color);
                    }
                    if (i == 1) 
                    {
                        m_Material->setType(Wiwa::Material::MaterialType::textured);
                    }
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            
            ImGui::EndCombo();
        }
        if (ImGui::Button("Save"))
        {
            Wiwa::JSONDocument matFile;
            matFile.AddMember("texture", m_Material->getTexturePath());
            matFile.AddMember("colorR", m_Material->getColor().r);
            matFile.AddMember("colorG", m_Material->getColor().g);
            matFile.AddMember("colorB", m_Material->getColor().b);
            matFile.AddMember("colorA", m_Material->getColor().a);
            matFile.AddMember("diffuseR", m_Material->getSettings().diffuse.r);
            matFile.AddMember("diffuseG", m_Material->getSettings().diffuse.g);
            matFile.AddMember("diffuseB", m_Material->getSettings().diffuse.b);
            matFile.AddMember("specularR", m_Material->getSettings().specular.r);
            matFile.AddMember("specularG", m_Material->getSettings().specular.g);
            matFile.AddMember("specularB", m_Material->getSettings().specular.b);
            matFile.AddMember("shininess", m_Material->getSettings().shininess);
            matFile.AddMember("type", (int)m_Material->getType());
            matFile.save_file(s_Path.string().c_str());
        }

    }
    else
    {
       TextCentered("Select a material to begin editing");
    }

   
	ImGui::End();
    ImGui::Begin("Light Debugger");
    Wiwa::DirectionalLight& directionalLight = Wiwa::Application::Get().GetRenderer3D().getFrameBuffer().getDirectionalLight();
    ImGui::DragFloat3("Direction", glm::value_ptr(directionalLight.Direction));
    ImGui::ColorEdit3("Ambient", glm::value_ptr(directionalLight.Ambient));
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(directionalLight.Diffuse));
    ImGui::ColorEdit3("Specular", glm::value_ptr(directionalLight.Specular));
    Wiwa::Application::Get().GetRenderer3D().getFrameBuffer().setLight(directionalLight);

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


    ImGui::End();
}



void MaterialPanel::OnEvent(Wiwa::Event& e)
{
    Wiwa::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MaterialChangeEvent>({ &MaterialPanel::OnMaterialChange, this });

}

bool MaterialPanel::OnMaterialChange(MaterialChangeEvent& e)
{
    m_Material = Wiwa::Resources::GetResourceById<Wiwa::Material>(e.GetResourceId());
    Wiwa::JSONDocument matFile(m_Material->getMaterialPath());
    if (matFile.HasMember("texture"))
        m_Material->setTexture(matFile["texture"].get<const char*>());
    s_Path = m_Material->getMaterialPath();
    return true;
}
