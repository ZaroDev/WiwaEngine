#include "wipch.h"

#include "MeshViewPanel.h"
#include "../../ShadingView.h"

#include <imgui.h>

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/Application.h>
#include <Wiwa/Renderer2D.h>

MeshViewPanel::MeshViewPanel()
    : Panel("Mesh view")
{
    
    //m_Shadings.push_back(new ShadingView("Shading", true));
    //m_Shadings.push_back(new ShadingView("Wireframe", false));
    //m_Shadings.push_back(new ShadingView("Light", false));
    //m_Shadings.push_back(new ShadingView("XD", false));
}

MeshViewPanel::~MeshViewPanel()
{
}

void MeshViewPanel::Draw()
{
    ImGui::Begin(name, &active, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Shading"))
        {
            //for (auto c : m_Shadings)
            //{
            //    if (ImGui::MenuItem(c->name, "", &c->active))
            //    {
            //        WI_INFO("{0}, {1}", c->name, c->active);
            //    }
            //}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();


        //TODO: Render the scene


        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                std::wstring ws(path);
                std::string pathS(ws.begin(), ws.end());
                //TODO: Load the scene with the path
                WI_INFO("Trying to load payload at path {0}", pathS.c_str());
            }

            ImGui::EndDragDropTarget();
        }

    }

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    Wiwa::Size2i resolution = Wiwa::Application::Get().GetTargetResolution();
    Wiwa::Size2f scales = { viewportPanelSize.x / (float)resolution.w, viewportPanelSize.y / (float)resolution.h };

    float scale = scales.x < scales.y ? scales.x : scales.y;

    ImVec2 isize = { resolution.w * scale, resolution.h * scale };

    ImTextureID tex = (ImTextureID)(intptr_t)Wiwa::Application::Get().GetRenderer2D().getColorBufferTexture();

    //ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - isize.x) / 2, (viewportPanelSize.y - isize.y) / 2));
    ImGui::Image(tex, isize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}
