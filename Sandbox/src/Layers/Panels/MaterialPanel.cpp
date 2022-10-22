#include <wipch.h>
#include "MaterialPanel.h"

#include <Wiwa/Resources.h>

#include <imgui.h>

#include <Wiwa/utilities/json/JSONDocument.h>

Wiwa::Material* s_Material = nullptr;
std::filesystem::path s_Path = "";

MaterialPanel::MaterialPanel()
	: Panel("Material Editor")
{
}

MaterialPanel::~MaterialPanel()
{
}

void MaterialPanel::Draw()
{
	ImGui::Begin(name, &active);

    
    if (s_Material)
    {
        ImGui::Image((ImTextureID)(intptr_t)s_Material->getTextureId(), { 64, 64 });

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                std::wstring ws(path);
                std::string pathS(ws.begin(), ws.end());
                std::filesystem::path p = pathS;
                if (p.extension() == ".png" && s_Material)
                {  
                    s_Material->setTexture(pathS.c_str());
                }

            }
            ImGui::EndDragDropTarget();

        }
        ImGui::Text(s_Material->getTexturePath());
        static glm::vec4 color = { s_Material->getColor().r, s_Material->getColor().g,s_Material->getColor().b , s_Material->getColor().a };
        ImGui::ColorEdit4("Color", glm::value_ptr(color));
        s_Material->setColor({ color.r, color.g, color.b, color.a });
        const char* types[] = { "Color", "Textured" };
        static const char* currentItem = s_Material->getType() == Wiwa::Material::MaterialType::color ? types[0] : types[1];
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
                        s_Material->setType(Wiwa::Material::MaterialType::color);
                    }
                    if (i == 1) 
                    {
                        s_Material->setType(Wiwa::Material::MaterialType::textured);
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
            matFile.AddMember("texture", s_Material->getTexturePath());
            matFile.AddMember("colorR", s_Material->getColor().r);
            matFile.AddMember("colorG", s_Material->getColor().g);
            matFile.AddMember("colorB", s_Material->getColor().b);
            matFile.AddMember("colorA", s_Material->getColor().a);
            matFile.AddMember("type", (int)s_Material->getType());
            matFile.save_file(s_Path.string().c_str());
        }

    }
    else
    {
        ImGui::Text("Drag a material to begin editing");
    }

   
	ImGui::End();
}

void MaterialPanel::SetMaterial(const char* file)
{
    ResourceId matId = Wiwa::Resources::Load<Wiwa::Material>(file);
    s_Material = Wiwa::Resources::GetResourceById<Wiwa::Material>(matId);
    s_Path = file;
    Wiwa::JSONDocument matFile(file);
    if (matFile.HasMember("texture"))
        s_Material->setTexture(matFile["texture"].get<const char*>());
}
