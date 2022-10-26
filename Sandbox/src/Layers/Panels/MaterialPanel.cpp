#include <wipch.h>
#include "MaterialPanel.h"

#include <Wiwa/Resources.h>

#include <Wiwa/utilities/json/JSONDocument.h>

#include "../../Utils/ImGuiWidgets.h"

#include <Wiwa/Renderer3D.h>
#include <Wiwa/Application.h>


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
        ImGui::Image((ImTextureID)(intptr_t)m_Material->getSpecularId(), { 64, 64 });
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
                    m_Material->setSpecular(pathS.c_str());
                }

            }
            ImGui::EndDragDropTarget();

        }

        glm::vec4 color = { m_Material->getColor().r, m_Material->getColor().g,m_Material->getColor().b , m_Material->getColor().a };
        ImGui::ColorEdit4("Color", glm::value_ptr(color));
        m_Material->setColor({ color.r, color.g, color.b, color.a });
        const char* types[] = { "Color", "Textured" };
        static const char* currentItem = m_Material->getType() == Wiwa::Material::MaterialType::color ? types[0] : types[1];
        
        Wiwa::Material::MaterialSettings& settings = m_Material->getSettings();
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(settings.diffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(settings.specular));
        ImGui::DragFloat("Shininess", &settings.shininess, 0.1f, 0, 512);
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
            matFile.AddMember("type", (int)m_Material->getType());
            matFile.save_file(s_Path.string().c_str());
        }

    }
    else
    {
       TextCentered("Select a material to begin editing");
    }

   
	ImGui::End();
    ImGui::Begin("Light Debbuger");
    Wiwa::Light& settings = Wiwa::Application::Get().GetRenderer3D().getFrameBuffer().getLight();
    ImGui::DragFloat3("Position", glm::value_ptr(settings.Position));
    ImGui::ColorEdit3("Ambient", glm::value_ptr(settings.Ambient));
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(settings.Diffuse));
    ImGui::ColorEdit3("Specular", glm::value_ptr(settings.Specular));
    Wiwa::Application::Get().GetRenderer3D().getFrameBuffer().setLight(settings);
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

    return true;
}
