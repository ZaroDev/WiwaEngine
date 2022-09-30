#include "ScenePanel.h"

#include <imgui.h>
ScenePanel::ScenePanel()
    : Panel("Scene")
{
    
    m_Shadings.push_back(new ShadingView("Shading", true));
    m_Shadings.push_back(new ShadingView("Wireframe", false));
    m_Shadings.push_back(new ShadingView("Light", false));
    m_Shadings.push_back(new ShadingView("XD", false));
}

ScenePanel::~ScenePanel()
{
}

void ScenePanel::Draw()
{
    ImGui::Begin(name, &active, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Shading"))
        {
            for (auto c : m_Shadings)
            {
                if (ImGui::MenuItem(c->name, "", &c->active))
                {
                    WI_INFO("{0}, {1}", c->name, c->active);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();


        //TODO: Render the scene


        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                //TODO: Load the scene with the path
                WI_INFO("Trying to load payload at path");
            }

            ImGui::EndDragDropTarget();
        }

    }
    ImGui::End();
}
