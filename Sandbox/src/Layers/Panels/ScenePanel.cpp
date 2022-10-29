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
#include <Wiwa/scene/Scene.h>
#include <Wiwa/scene/SceneManager.h>
#include "../EditorLayer.h"

ScenePanel::ScenePanel(EditorLayer* instance)
    : Panel("Scene", instance)
{
    m_Shadings.push_back(new ShadingView("Depth Test", Wiwa::Renderer3D::Options::DEPTH_TEST, false));
    m_Shadings.push_back(new ShadingView("Cull face", Wiwa::Renderer3D::Options::CULL_FACE, false));
    m_Shadings.push_back(new ShadingView("Lighting", Wiwa::Renderer3D::Options::LIGHTING, false));
    m_Shadings.push_back(new ShadingView("Color material", Wiwa::Renderer3D::Options::COLOR_MATERIAL, false));
    m_Shadings.push_back(new ShadingView("Texture 2D", Wiwa::Renderer3D::Options::TEXTURE_2D, false));
    m_Shadings.push_back(new ShadingView("Wireframe", Wiwa::Renderer3D::Options::WIREFRAME, false));

    Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
    float ar = res.w / (float)res.h;
    nearPlane = 0.1f;
    farPlane = 1000.0f;

    m_Camera.SetPerspective(45.0f, ar, nearPlane, farPlane);
    m_Camera.setPosition({ 0.0f, 1.0f, 5.0f });
    m_Camera.lookat({ 0.0f, 0.0f, 0.0f });
    // Camera control
    camSpeed = 0.005f;
    sensitivity = 0.5f;

    yaw = -90.0f;
    pitch = 0.0f;
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
        if (ImGui::BeginMenu("Camera"))
        {
            ImGui::SliderFloat("Camera speed", &camSpeed, 0.001f, 1.0f);
            ImGui::SliderFloat("Camera sensitivity", &sensitivity, 0.01f, 5.0f);
            if (ImGui::InputFloat("Near Plane", &nearPlane, 0.1f, 1.0f))
            {
                m_Camera.setPlanes(nearPlane, farPlane);
            }

            if(ImGui::InputFloat("Far Plane", &farPlane, 0.1f, 1.0f))
            {
                m_Camera.setPlanes(nearPlane, farPlane);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    //TODO: Render the scene
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    Wiwa::Size2i resolution = Wiwa::Application::Get().GetTargetResolution();
    float ar = resolution.w / (float)resolution.h;
    Wiwa::Size2f scales = { viewportPanelSize.x / (float)resolution.w, viewportPanelSize.y / (float)resolution.h };

    float scale = scales.x < scales.y ? scales.x : scales.y;

    ImVec2 isize = { resolution.w * scale, resolution.h * scale };
    
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
        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::F) && m_EntSelected != -1)
        {
            float radius = 7.0f;
            glm::vec3 direction = {};
            direction.x = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw)) + m_SelectedTransform->position.x;
            direction.y = radius * sin(glm::radians(pitch)) * sin(glm::radians(yaw)) + m_SelectedTransform->position.y;
            direction.z = radius * cos(glm::radians(yaw)) + m_SelectedTransform->position.z;
            m_Camera.setPosition({ direction.x, direction.y, direction.z });
            m_Camera.lookat(m_SelectedTransform->position);
        }
        // Check if right click was pressed
        if (Wiwa::Input::IsMouseButtonPressed(1)) {
            // Check if relative motion is not 0
            if (rel2f != Wiwa::Vector2f::Zero()) {
                float xoffset = -rel2f.x * sensitivity;
                float yoffset = rel2f.y * sensitivity;

                yaw += xoffset;
                pitch += yoffset;
                if (Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftAlt) && m_EntSelected != -1)
                {
                    float radius = 7.0f;
                    glm::vec3 direction = {};
                    direction.x = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw)) + m_SelectedTransform->position.x;
                    direction.y = radius * sin(glm::radians(pitch)) * sin(glm::radians(yaw)) + m_SelectedTransform->position.y;
                    direction.z = radius * cos(glm::radians(yaw)) + m_SelectedTransform->position.z;
                    m_Camera.setPosition({ direction.x, direction.y, direction.z });
                    m_Camera.lookat(m_SelectedTransform->position);
                }
                else
                {
                    if (pitch > 89.0f) pitch = 89.0f;
                    if (pitch < -89.0f) pitch = -89.0f;

                    glm::vec3 direction;
                    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    direction.y = sin(glm::radians(pitch));
                    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

                    glm::vec3 front = glm::normalize(direction);
                    m_Camera.setFront({ front.x, front.y, front.z });
                }
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftShift))
                camFastSpeed = camSpeed * 2;
            else
                camFastSpeed = camSpeed;
            float fov = m_Camera.getFOV();
            if (m_Scroll > 0)
                fov -= 10;
            else if (m_Scroll < 0)
                fov += 10;

            CLAMP(fov, 1, 120);
            m_Scroll = 0.0f;
            m_Camera.setFOV(fov);
            // Camera movement
            glm::vec3 campos = m_Camera.getPosition();

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::W)) {
                campos += m_Camera.getFront() * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::S)) {
                campos -= m_Camera.getFront() * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::A)) {
                campos -= glm::normalize(glm::cross(m_Camera.getFront(), m_Camera.getUp())) * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::D)) {
                campos += glm::normalize(glm::cross(m_Camera.getFront(), m_Camera.getUp())) * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::Q)) {
                campos += m_Camera.getUp() * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::E)) {
                campos -= m_Camera.getUp() * camFastSpeed;
            }
            m_Camera.setPosition({ campos.x, campos.y, campos.z }); 
        }
    }
    Wiwa::Application::Get().GetRenderer3D().SetActiveCamera(m_Camera);

    static bool grid = true;

    ImTextureID tex = (ImTextureID)(intptr_t)Wiwa::Application::Get().GetRenderer3D().getColorBufferTexture();
    ImVec2 cpos = ImGui::GetCursorPos();
    cpos.x = (viewportPanelSize.x - isize.x) / 2;
    ImGui::SetCursorPos(cpos);
    //Wiwa::Application::Get().GetRenderer3D().RenderGrid();
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
        float y = cpos.y + 5.0f;
        float x = cpos.x + 5.0f;
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

    if (m_EntSelected != -1)
    {
        m_GizmoType = instance->GetGizmo();
        if (m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImVec2 winPos = ImGui::GetWindowPos();
            ImVec2 cursorPos = ImGui::GetCursorPos();
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(rectPos.x, rectPos.y, isize.x, isize.y);

            glm::mat4 cameraView = Wiwa::Application::Get().GetRenderer3D().GetView();
            const glm::mat4& cameraProjection = m_Camera.getProjection();
            //TODO: Change to get the transform of the entity

            Wiwa::EntityManager& entMan = Wiwa::Application::Get().GetEntityManager();
            m_SelectedTransform = entMan.GetComponent<Wiwa::Transform3D>(m_EntSelected);
            if (m_SelectedTransform)
            {
                glm::mat4 transform(1.0f);

                transform = glm::translate(transform, glm::vec3(m_SelectedTransform->position.x, m_SelectedTransform->position.y, m_SelectedTransform->position.z));
                transform = glm::rotate(transform, m_SelectedTransform->rotation.x, { 1,0,0 });
                transform = glm::rotate(transform, m_SelectedTransform->rotation.y, { 0,1,0 });
                transform = glm::rotate(transform, m_SelectedTransform->rotation.z, { 0,0,1 });
                transform = glm::scale(transform, glm::vec3(m_SelectedTransform->scale.x, m_SelectedTransform->scale.y, m_SelectedTransform->scale.z));

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

                    Wiwa::Vector3f newTranslation = { translation[0], translation[1], translation[2] };
                    Wiwa::Vector3f newRotation = { rotation[0], rotation[1], rotation[2] };
                    Wiwa::Vector3f newScale = { scale[0], scale[1], scale[2] };

                    m_SelectedTransform->position = newTranslation;
                    m_SelectedTransform->rotation = newRotation;
                    m_SelectedTransform->scale = newScale;
                }
            }
        }
    }

    ImGui::End();
}

void ScenePanel::OnEvent(Wiwa::Event& e)
{
    Wiwa::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Wiwa::MouseScrolledEvent>({ &ScenePanel::OnMouseScrollEvent, this });
    dispatcher.Dispatch<EntityChangeEvent>({ &ScenePanel::OnEntityChange, this });
}

bool ScenePanel::OnMouseScrollEvent(Wiwa::MouseScrolledEvent& e)
{
    m_Scroll = e.GetYOffset();
    return false;
}

bool ScenePanel::OnEntityChange(EntityChangeEvent& e)
{
    m_EntSelected = (int)e.GetResourceId();
    return false;
}


