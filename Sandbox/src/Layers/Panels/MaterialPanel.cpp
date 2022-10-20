#include <wipch.h>
#include "MaterialPanel.h"

#include <Wiwa/Resources.h>

#include <imgui.h>

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
                ResourceId matId = Wiwa::Resources::Load<Wiwa::Material>(pathS.c_str());
                m_Material = Wiwa::Resources::GetResourceById<Wiwa::Material>(matId);
            }
        }

        ImGui::EndDragDropTarget();
    }
	ImGui::End();
}
