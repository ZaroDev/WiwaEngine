#include "wipch.h"

#include "ScenePanel.h"
#include "../../ShadingView.h"


#include "ImGuizmo.h"

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/Application.h>
#include <Wiwa/Renderer2D.h>
#include <Wiwa/Renderer3D.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/Input.h>

#include "InspectorPanel.h"

ScenePanel::ScenePanel()
    : Panel("Scene")
{
    
    m_Shadings.push_back(new ShadingView("Depth Test", Wiwa::Renderer3D::Options::DEPTH_TEST, false));
    m_Shadings.push_back(new ShadingView("Cull face", Wiwa::Renderer3D::Options::CULL_FACE, false));
    m_Shadings.push_back(new ShadingView("Lighting", Wiwa::Renderer3D::Options::LIGHTING, false));
    m_Shadings.push_back(new ShadingView("Color material", Wiwa::Renderer3D::Options::COLOR_MATERIAL, false));
    m_Shadings.push_back(new ShadingView("Texture 2D", Wiwa::Renderer3D::Options::TEXTURE_2D, false));
    m_Shadings.push_back(new ShadingView("Wireframe", Wiwa::Renderer3D::Options::WIREFRAME, false));

}

ScenePanel::~ScenePanel()
{
}

void ScenePanel::Draw()
{
    ImGui::Begin(name, &active, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {

        if (ImGui::BeginMenu("Options"))
        {
            for (auto c : m_Shadings)
            {
                if (ImGui::MenuItem(c->name, "", &c->active))
                {
                    if (c->active) Wiwa::Application::Get().GetRenderer3D().SetOption(c->glValue);
                    else Wiwa::Application::Get().GetRenderer3D().DisableOption(c->glValue);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    //TODO: Render the scene
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    Wiwa::Size2i resolution = Wiwa::Application::Get().GetTargetResolution();
    Wiwa::Size2f scales = { viewportPanelSize.x / (float)resolution.w, viewportPanelSize.y / (float)resolution.h };

    float scale = scales.x < scales.y ? scales.x : scales.y;

    ImVec2 isize = { resolution.w * scale, resolution.h * scale };

    ImTextureID tex = (ImTextureID)(intptr_t)Wiwa::Application::Get().GetRenderer2D().getColorBufferTexture();

    //ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - isize.x) / 2, (viewportPanelSize.y - isize.y) / 2));
    ImGui::Image(tex, isize, ImVec2(0, 1), ImVec2(1, 0));

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

    //Gizmos
    Wiwa::EntityManager& entityManager = Wiwa::Application::Get().GetEntityManager();
    uint32_t entId;
    if (InspectorPanel::GetCurrentEntity(entId))
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        glm::mat4 cameraView = Wiwa::Application::Get().GetRenderer3D().GetView();
        const glm::mat4& cameraProjection = Wiwa::Application::Get().GetRenderer3D().GetPersProjection();
        //TODO: Change to get the transform of the entity
        
        glm::mat4 transform(1);

        //Snaping
        bool snap = Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftControl);
        float snapValue = 0.5f; //Snap to 0.5m for translation/scale
        if(snap)
            WI_TRACE("Snap value true");
        float snapValues[3] = { snapValue, snapValue, snapValue };

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
            nullptr, snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing())
        {
            //TODO: Change the transform matrix!
        }
    }


    ImGui::End();
}
