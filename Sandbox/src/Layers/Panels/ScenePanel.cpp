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
    }
    ImGui::End();
}
