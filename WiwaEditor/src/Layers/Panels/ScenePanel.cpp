#include "wipch.h"

#include "ScenePanel.h"
#include "../../ShadingView.h"


#include "ImGuizmo.h"

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/core/Renderer3D.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/core/Input.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>

#include "InspectorPanel.h"

#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/math/Math.h>


#include <glm/gtx/matrix_decompose.hpp>
#include <Wiwa/scene/Scene.h>
#include <Wiwa/scene/SceneManager.h>
#include "../EditorLayer.h"

ScenePanel::ScenePanel(EditorLayer* instance)
    : Panel("Scene", instance)
{
    m_Shadings.push_back(new ShadingView("Depth Test", Wiwa::Renderer3D::Options::DEPTH_TEST, true));
    m_Shadings.push_back(new ShadingView("Cull face", Wiwa::Renderer3D::Options::CULL_FACE, true));
    m_Shadings.push_back(new ShadingView("Lighting", Wiwa::Renderer3D::Options::LIGHTING, false));
    m_Shadings.push_back(new ShadingView("Color material", Wiwa::Renderer3D::Options::COLOR_MATERIAL, false));
    m_Shadings.push_back(new ShadingView("Texture 2D", Wiwa::Renderer3D::Options::TEXTURE_2D, false));
    m_Shadings.push_back(new ShadingView("Wireframe", Wiwa::Renderer3D::Options::WIREFRAME, false));

    Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
    float ar = res.w / (float)res.h;
    nearPlane = 0.1f;
    farPlane = 10000.0f;

    m_Camera = Wiwa::CameraManager::editorCamera;
    m_Camera->SetPerspective(60, ar, nearPlane, farPlane);
    m_Camera->setPosition({ 0.0f, 1.0f, 5.0f });
    m_Camera->lookat({ 0.0f, 0.0f, 0.0f });
    // Camera control
    camSpeed = 0.005f;
    sensitivity = 0.5f;

    m_ScrollSpeed = 10.0f;

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
                m_Camera->setPlanes(nearPlane, farPlane);
            }

            if(ImGui::InputFloat("Far Plane", &farPlane, 0.1f, 1.0f))
            {
                m_Camera->setPlanes(nearPlane, farPlane);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    Wiwa::Size2i resolution = Wiwa::Application::Get().GetTargetResolution();
    float ar = resolution.w / (float)resolution.h;
    Wiwa::Size2f scales = { viewportPanelSize.x / (float)resolution.w, viewportPanelSize.y / (float)resolution.h };

    float scale = scales.x < scales.y ? scales.x : scales.y;

    ImVec2 isize = { resolution.w * scale, resolution.h * scale };
    ImVec2 cpos = ImGui::GetCursorPos();
    cpos.x = (viewportPanelSize.x - isize.x) / 2;
    Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
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
            m_Camera->setPosition({ direction.x, direction.y, direction.z });
            m_Camera->lookat(m_SelectedTransform->position);
        }

        // Scene zoom
        if (m_Scroll != 0.0f) {
            float fov = m_Camera->getFOV();
            fov += m_Scroll * m_ScrollSpeed;
            CLAMP(fov, 1, 120);
            m_Camera->setFOV(fov);
        }

        if(Wiwa::Input::IsMouseButtonPressed(0) && !ImGuizmo::IsUsing())
        {
            glm::vec3 out_dir;
            glm::vec3 out_origin;
            rpos.y -= isize.y;
            rpos.y = glm::abs(rpos.y);
            
            Wiwa::Math::ScreenPosToWorldRay(rpos.x, rpos.y , isize.x, isize.y, m_Camera->getView(), m_Camera->getProjection(), out_origin, out_dir);
            float minDist = FLT_MAX;
            int id = -1;
            for (size_t i = 0; i < entityManager.GetEntityCount(); i++) 
            {
                if (!entityManager.HasComponent<Wiwa::Mesh>(i))
                    continue;
                Wiwa::Model* model = Wiwa::Resources::GetResourceById<Wiwa::Model>(entityManager.GetComponent<Wiwa::Mesh>(i)->meshId);
                Wiwa::Transform3D* trs = entityManager.GetComponent<Wiwa::Transform3D>(i);
                glm::mat4 transform(1.0f);
                glm::vec3 scale = glm::vec3(trs->scale.x, trs->scale.y, trs->scale.z);
                transform = glm::translate(transform, glm::vec3(trs->position.x, trs->position.y, trs->position.z));
                transform = glm::rotate(transform, trs->rotation.x, { 1,0,0 });
                transform = glm::rotate(transform, trs->rotation.y, { 0,1,0 });
                transform = glm::rotate(transform, trs->rotation.z, { 0,0,1 });
                transform = glm::scale(transform, scale);
                float intersectDist = 0.0f;
                if (Wiwa::Math::TestRayOBBIntersection(
                    out_origin,
                    out_dir,
                    model->boundingBox.getMin() * scale,
                    model->boundingBox.getMax() * scale, 
                    transform,
                    intersectDist
                ))
                {
                    if (i == 0)
                    {
                        minDist = intersectDist;
                        id = i;
                    }
                    else if (intersectDist < minDist)
                    {
                        minDist = intersectDist;
                        id = i;
                    }
                }
            }
            if (id >= 0)
                m_EntSelected = id;
            else
                m_EntSelected = -1;

            EntityChangeEvent event((uint32_t)id);
            Action<Wiwa::Event&> act = { &EditorLayer::OnEvent, instance };
            act(event);
        }
        // Check if right click was pressed
        if (Wiwa::Input::IsMouseButtonPressed(1)) {
            // Check if relative motion is not 0
            if (rel2f != Wiwa::Vector2f::Zero()) {
                float xoffset = -rel2f.x * sensitivity;
                float yoffset = rel2f.y * sensitivity;

                yaw += xoffset;
                pitch += yoffset;

                if (pitch > 89.0f) pitch = 89.0f;
                if (pitch < -89.0f) pitch = -89.0f;

                if (Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftAlt) && m_EntSelected != -1)
                {
                    float radius = 7.0f;
                    glm::vec3 direction = {};
                    direction.x = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw)) + m_SelectedTransform->position.x;
                    direction.y = radius * sin(glm::radians(pitch)) * sin(glm::radians(yaw)) + m_SelectedTransform->position.y;
                    direction.z = radius * cos(glm::radians(yaw)) + m_SelectedTransform->position.z;
                    m_Camera->setPosition({ direction.x, direction.y, direction.z });
                    m_Camera->lookat(m_SelectedTransform->position);
                }
                else
                {
                    glm::vec3 direction;
                    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    direction.y = sin(glm::radians(pitch));
                    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

                    glm::vec3 front = glm::normalize(direction);
                    m_Camera->setFront({ front.x, front.y, front.z });
                }
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftShift))
                camFastSpeed = camSpeed * 2;
            else
                camFastSpeed = camSpeed;
            
            // Camera movement
            glm::vec3 campos = m_Camera->getPosition();

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::W)) {
                campos += m_Camera->getFront() * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::S)) {
                campos -= m_Camera->getFront() * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::A)) {
                campos -= glm::normalize(glm::cross(m_Camera->getFront(), m_Camera->getUp())) * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::D)) {
                campos += glm::normalize(glm::cross(m_Camera->getFront(), m_Camera->getUp())) * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::Q)) {
                campos += m_Camera->getUp() * camFastSpeed;
            }

            if (Wiwa::Input::IsKeyPressed(Wiwa::Key::E)) {
                campos -= m_Camera->getUp() * camFastSpeed;
            }
            m_Camera->setPosition({ campos.x, campos.y, campos.z }); 
        }
    }
    
    m_Scroll = 0.0f;


    static bool grid = true;

    
    //Wiwa::Application::Get().GetRenderer3D().SetActiveCamera(m_Camera);

    ImTextureID tex = (ImTextureID)(intptr_t)m_Camera->frameBuffer->getColorBufferTexture();
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
            std::filesystem::path p = pathS;
            if (p.extension() == ".fbx" || p.extension() == ".FBX")
            {
                EntityId newEnt = entityManager.CreateEntity(p.stem().string().c_str());
                Wiwa::Mesh mesh;
                entityManager.AddComponent<Wiwa::Transform3D>(newEnt, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
                mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(pathS.c_str());
                mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>("resources/materials/default_material.wimaterial");
                entityManager.AddComponent<Wiwa::Mesh>(newEnt, mesh);
                entityManager.ApplySystem<Wiwa::MeshRenderer>(newEnt);
            }

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


    if (m_EntSelected != -1)
    {
        m_SelectedTransform = entityManager.GetComponent<Wiwa::Transform3D>(m_EntSelected);
        m_GizmoType = instance->GetGizmo();
        if (m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImVec2 winPos = ImGui::GetWindowPos();
            ImVec2 cursorPos = ImGui::GetCursorPos();
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(rectPos.x, rectPos.y, isize.x, isize.y);

            glm::mat4 cameraView = m_Camera->getView();
            const glm::mat4& cameraProjection = m_Camera->getProjection();

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
                    Wiwa::Vector3f newRotation = { glm::radians(rotation[0]), glm::radians(rotation[1]), glm::radians(rotation[2]) };
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

void ScenePanel::Update()
{
   
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


