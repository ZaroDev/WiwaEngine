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

#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/utilities/render/Model.h>


#include <gtx/matrix_decompose.hpp>

ScenePanel::ScenePanel()
    : Panel("Scene")
{
    m_Shadings.push_back(new ShadingView("Depth Test", Wiwa::Renderer3D::Options::DEPTH_TEST, false));
    m_Shadings.push_back(new ShadingView("Cull face", Wiwa::Renderer3D::Options::CULL_FACE, false));
    m_Shadings.push_back(new ShadingView("Lighting", Wiwa::Renderer3D::Options::LIGHTING, false));
    m_Shadings.push_back(new ShadingView("Color material", Wiwa::Renderer3D::Options::COLOR_MATERIAL, false));
    m_Shadings.push_back(new ShadingView("Texture 2D", Wiwa::Renderer3D::Options::TEXTURE_2D, false));
    m_Shadings.push_back(new ShadingView("Wireframe", Wiwa::Renderer3D::Options::WIREFRAME, false));

    Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
    float ar = res.w / (float)res.h;

}

ScenePanel::~ScenePanel()
{
}

void ScenePanel::Draw()
{
    ImGui::Begin(name, &active, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {

        if (ImGui::BeginMenu("Render"))
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
        if(ImGui::BeginMenu("Options"))
        {
            Wiwa::Vector4f color = Wiwa::Application::Get().GetRenderColor();
            glm::vec4 col = { color.r, color.g, color.b, color.a };
            if (ImGui::ColorEdit4("Render color", glm::value_ptr(col)))
            {
                color = { col.r, col.g, col.b, col.a };
                Wiwa::Application::Get().SetRenderColor(color);
            }
            ImGui::MenuItem("Show FPS", "", &m_ShowFPS);
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

    ImTextureID tex = (ImTextureID)(intptr_t)Wiwa::Application::Get().GetRenderer3D().getColorBufferTexture();
    ImGui::Image(tex, isize, ImVec2(0, 1), ImVec2(1, 0));
    // Horizontal-align viewport
    ImVec2 cpos = ImGui::GetCursorPos();
    cpos.x = (viewportPanelSize.x - isize.x) / 2;
    ImGui::SetCursorPos(cpos);

    if (ImGui::IsWindowHovered())
    {
        // Calculate mouse position in viewport (0 to 1)
        ImVec2 mpos = ImGui::GetMousePos();
        ImVec2 cspos = ImGui::GetCursorScreenPos();

        ImVec2 rpos = { mpos.x - cspos.x, mpos.y - cspos.y };
        CLAMP(rpos.x, 0.0f, isize.x);
        CLAMP(rpos.y, 0.0f, isize.y);

        Wiwa::Vector2f v2f = { rpos.x / (float)isize.x, rpos.y / (float)isize.y };
        Wiwa::Vector2f rel2f = lastPos - v2f;
        rel2f.x /= rel2f.x == 0.0f ? 1.0f : abs(rel2f.x);
        rel2f.y /= rel2f.y == 0.0f ? 1.0f : abs(rel2f.y);

        lastPos = v2f;

        // Check if right click was pressed
        if (Wiwa::Input::IsMouseButtonPressed(1))
        {
            /* Wiwa::Input::SetCursorVisible(false);
             if (m_FirstClick)
             {
                 Wiwa::Input::SetMousePos((cspos.x + isize.x) / 2, (cspos.y + isize.y) / 2);
                 m_FirstClick = false;
             }*/

             // Check if relative motion is not 0
            if (rel2f != Wiwa::Vector2f::Zero())
            {
                float xoffset = -rel2f.x * sensitivity;
                float yoffset = rel2f.y * sensitivity;

                yaw += xoffset;
                pitch += yoffset;

                if (pitch > 89.0f) pitch = 89.0f;
                if (pitch < -89.0f) pitch = -89.0f;

                glm::vec3 direction;
                direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                direction.y = sin(glm::radians(pitch));
                direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

                glm::vec3 front = glm::normalize(direction);
                m_Camera.setFront({ front.x, front.y, front.z });

                /*Wiwa::Input::SetMousePos((cspos.x + isize.x) / 2, (cspos.y + isize.y) / 2);*/
            }
            else
            {
                Wiwa::Input::SetCursorVisible(true);
                m_FirstClick = true;
            }
        }

        // Camera movement
        glm::vec3 campos = m_Camera.getPosition();

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::W)) {
            campos += m_Camera.getFront() * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::S)) {
            campos -= m_Camera.getFront() * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::A)) {
            campos -= glm::normalize(glm::cross(m_Camera.getFront(), m_Camera.getUp())) * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::D)) {
            campos += glm::normalize(glm::cross(m_Camera.getFront(), m_Camera.getUp())) * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftShift)) {
            campos -= m_Camera.getUp() * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::Q)) {
            campos += m_Camera.getUp() * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::E)) {
            campos -= m_Camera.getUp() * camSpeed;
        }

        m_Camera.setPosition({ campos.x, campos.y, campos.z });
    }

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
    //FPS widget
    ImVec2 rectPos = ImGui::GetItemRectMin();
    if (m_ShowFPS)
    {
        ImVec2 rectSize(rectPos.x + 150.0f, rectPos.y + 50.0f);
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(rectPos.x, rectPos.y),
            rectSize,
            IM_COL32(30, 30, 30, 128)
        );

        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(rectPos.x, rectPos.y),
            rectSize,
            IM_COL32(255, 255, 255, 30)
        );
        float y = 70.0f;
        float x = 25.0f;
        ImGui::SetCursorPos(ImVec2(x, y));
        ImGui::TextColored(ImColor(255, 255, 255, 128), "FPS");
        ImGui::SetCursorPos(ImVec2(x + 60.0f, y));
        ImGui::TextColored(ImColor(255, 255, 255, 128), "%.f FPS", ImGui::GetIO().Framerate);
        
        ImGui::SetCursorPos(ImVec2(x, y + 20.0f));
        ImGui::TextColored(ImColor(255, 255, 255, 128), "Frame time");
        ImGui::SetCursorPos(ImVec2(x + 70.0f, y + 20.0f));
        ImGui::TextColored(ImColor(255, 255, 255, 128), "%.3f ms", 1000 / ImGui::GetIO().Framerate);
    }
    //Gizmos
    Wiwa::EntityManager& entityManager = Wiwa::Application::Get().GetEntityManager();
    uint32_t entId;
    if (InspectorPanel::GetCurrentEntity(entId))
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(winPos.x, winPos.y, isize.x, isize.y);

        glm::mat4 cameraView = Wiwa::Application::Get().GetRenderer3D().GetView();
        const glm::mat4& cameraProjection = Wiwa::Application::Get().GetRenderer3D().GetPersProjection();
        //TODO: Change to get the transform of the entity
        
        Wiwa::EntityManager& entMan = Wiwa::Application::Get().GetEntityManager();
        Wiwa::Transform3D* transformCmp = entMan.GetComponent<Wiwa::Transform3D>(entId);
        if (transformCmp)
        {
            glm::mat4 transform(1.0f);

            transform = glm::translate(transform, glm::vec3(transformCmp->position.x, transformCmp->position.y, transformCmp->position.z));
            transform = glm::rotate(transform, transformCmp->rotation.x, { 1,0,0 });
            transform = glm::rotate(transform, transformCmp->rotation.y, { 0,1,0 });
            transform = glm::rotate(transform, transformCmp->rotation.z, { 0,0,1 });
            transform = glm::scale(transform, glm::vec3(transformCmp->scale.x, transformCmp->scale.y, transformCmp->scale.z));

            //Snaping
            bool snap = Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftControl);
            float snapValue = 0.5f; //Snap to 0.5m for translation/scale

            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                ////TODO: Change the transform matrix!
                float translation[3], rotation[3], scale[3];

                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), translation, rotation, scale);

                Wiwa::Vector3f newTranslation = { translation[0], translation[1], translation[2]};
                Wiwa::Vector3f newRotation = { rotation[0], rotation[1], rotation[2]};
                Wiwa::Vector3f newScale = { scale[0], scale[1], scale[2]};

                transformCmp->position = newTranslation;
                transformCmp->rotation = newRotation;
                transformCmp->scale = newScale;
            }
        }

    }

    ImGui::End();
}
