#include "wipch.h"

#include "ScenePanel.h"
#include "../../ShadingView.h"


#include "ImGuizmo.h"

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/core/Application.h>
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
#include "../../Entities.h"

#include <Wiwa/core/Renderer2D.h>

#include <Wiwa/utilities/filesystem/FileSystem.h>

ScenePanel::ScenePanel(EditorLayer* instance)
    : Panel("Scene", instance)
{
    m_LocalMode = false;
    m_Shadings.push_back(new ShadingView("Depth Test", Wiwa::Renderer3D::Options::DEPTH_TEST, true));
    m_Shadings.push_back(new ShadingView("Cull face", Wiwa::Renderer3D::Options::CULL_FACE, true));
    m_Shadings.push_back(new ShadingView("Lighting", Wiwa::Renderer3D::Options::LIGHTING, false));
    m_Shadings.push_back(new ShadingView("Color material", Wiwa::Renderer3D::Options::COLOR_MATERIAL, false));
    m_Shadings.push_back(new ShadingView("Texture 2D", Wiwa::Renderer3D::Options::TEXTURE_2D, false));
    m_Shadings.push_back(new ShadingView("Wireframe", Wiwa::Renderer3D::Options::WIREFRAME, false));

   
    m_Camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;
   
    // Camera control
    camSpeed = 0.085f;
    sensitivity = 0.8f;
    nearPlane = m_Camera->getNear();
    farPlane = m_Camera->getFar();
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
    m_Camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;
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
        if (ImGui::BeginMenu("Options"))
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

            if (ImGui::InputFloat("Far Plane", &farPlane, 0.1f, 1.0f))
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
    
    //Wiwa::Application::Get().GetRenderer3D().SetActiveCamera(m_Camera);

    ImTextureID tex = (ImTextureID)(intptr_t)m_Camera->frameBuffer->getColorBufferTexture();
    //ImTextureID tex = (ImTextureID)(intptr_t)Wiwa::Application::Get().GetRenderer2D().getColorBufferTexture();

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
                std::filesystem::path src = Wiwa::FileSystem::RemoveFolderFromPath("assets", pathS);
                src.replace_extension();
                CreateEntityWithModelHierarchy(src.string().c_str());
            }
            if (p.extension() == ".wiscene")
            {
                EditorLayer::Get().LoadScene(pathS);
            }
        }

        ImGui::EndDragDropTarget();
    }
    ImVec2 rectPos = ImGui::GetItemRectMin();
    //FPS widget

    if (m_ShowFPS)
    {
        ImVec2 rectSize(rectPos.x + 150.0f, rectPos.y + 50.0f);
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(rectPos.x + 10.0f, rectPos.y),
            rectSize,
            IM_COL32(30, 30, 30, 128)
        );

        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(rectPos.x + 10.0f, rectPos.y),
            rectSize,
            IM_COL32(255, 255, 255, 30)
        );
        float y = cpos.y + 5.0f;
        float x = cpos.x + 15.0f;
        ImGui::SetCursorPos(ImVec2(x, y));
        ImGui::TextColored(ImColor(255, 255, 255, 128), "FPS");
        ImGui::SetCursorPos(ImVec2(x + 60.0f, y));
        ImGui::TextColored(ImColor(255, 255, 255, 128), "%.f FPS", 1000 / Wiwa::Time::GetRealDeltaTime());

        ImGui::SetCursorPos(ImVec2(x, y + 20.0f));
        ImGui::TextColored(ImColor(255, 255, 255, 128), "Frame time");
        ImGui::SetCursorPos(ImVec2(x + 70.0f, y + 20.0f));
        ImGui::TextColored(ImColor(255, 255, 255, 128), "%.3f ms", Wiwa::Time::GetRealDeltaTime());
    }
    //Gizmos
    if (m_EntSelected != -1)
    {
        m_SelectedTransform = entityManager.GetComponent<Wiwa::Transform3D>(m_EntSelected);
        m_GizmoType = instance->GetGizmo();
        if (m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);

            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(rectPos.x, rectPos.y, isize.x, isize.y);

            glm::mat4 cameraView = m_Camera->getView();
            const glm::mat4& cameraProjection = m_Camera->getProjection();

            if (m_SelectedTransform)
            {
                bool isParent = entityManager.GetEntityParent(m_EntSelected) == m_EntSelected;
                float tmpMatrix[16];
                float translation[3], rotation[3], scale[3];

                translation[0] = m_SelectedTransform->position.x;
                translation[1] = m_SelectedTransform->position.y;
                translation[2] = m_SelectedTransform->position.z;

                rotation[0] = m_SelectedTransform->rotation.x;
                rotation[1] = m_SelectedTransform->rotation.y;
                rotation[2] = m_SelectedTransform->rotation.z;

                scale[0] = m_SelectedTransform->scale.x;
                scale[1] = m_SelectedTransform->scale.y;
                scale[2] = m_SelectedTransform->scale.z;                

                ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, tmpMatrix);
                ImGuizmo::MODE mode = isParent ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
                //Snaping
                bool snap = Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftControl);
                float snapValue = 0.5f; //Snap to 0.5m for translation/scale

                if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                    snapValue = 45.0f;

                float snapValues[3] = { snapValue, snapValue, snapValue };

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                    (ImGuizmo::OPERATION)m_GizmoType, mode, tmpMatrix,
                    nullptr, snap ? snapValues : nullptr);

                if (ImGuizmo::IsUsing())
                {
                    ImGuizmo::DecomposeMatrixToComponents(tmpMatrix, translation, rotation, scale);

                    switch (m_GizmoType)
                    {
                    case ImGuizmo::OPERATION::TRANSLATE:
                    {
                        m_SelectedTransform->localPosition += Wiwa::Vector3f(translation[0], translation[1], translation[2]) - m_SelectedTransform->position;
                    }break;
                    case ImGuizmo::OPERATION::ROTATE: 
                    {
                        m_SelectedTransform->localRotation += Wiwa::Vector3f(rotation[0], rotation[1], rotation[2]) - m_SelectedTransform->rotation;
                    }break;
                    case ImGuizmo::OPERATION::SCALE: 
                    {
                        m_SelectedTransform->localScale += Wiwa::Vector3f(scale[0], scale[1], scale[2]) - m_SelectedTransform->scale;
                    }break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    if (ImGui::IsWindowHovered())
    {
        // Calculate mouse position in viewport (0 to 1)
        ImVec2 mpos = ImGui::GetMousePos();
        ImVec2 cspos = ImGui::GetCursorScreenPos();

        ImVec2 rpos = { mpos.x - rectPos.x, mpos.y - rectPos.y };
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

        if (Wiwa::Input::IsMouseButtonPressed(0) && !ImGuizmo::IsUsing())
        {
            glm::vec3 out_dir;
            glm::vec3 out_origin;

            // Position on screen with pixels, invert Y for left-down origin
            Wiwa::Vector2f pospixels = { lastPos.x * 1920, (1.0f - lastPos.y) * 1080 };

            // Send mouse position in pixels with left-down origin coordinates and game resolution size
            Wiwa::Math::ScreenPosToWorldRay(pospixels.x, pospixels.y, 1920, 1080, m_Camera->getView(), m_Camera->getProjection(), out_origin, out_dir);
            float minDist = FLT_MAX;
            int id = -1;
            std::vector<EntityId>* entities_alive = entityManager.GetEntitiesAlive();
            size_t entity_count = entities_alive->size();
            for (size_t i = 0; i < entity_count; i++)
            {
                EntityId eid = entities_alive->at(i);

                if (!entityManager.HasComponent<Wiwa::Mesh>(eid))
                    continue;
                Wiwa::Mesh* mesh = entityManager.GetComponent<Wiwa::Mesh>(eid);
                Wiwa::Model* model = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
                model = model->getModelAt(mesh->modelIndex);
                Wiwa::Transform3D* trs = entityManager.GetComponent<Wiwa::Transform3D>(eid);
                glm::vec3 scale = glm::vec3(trs->localScale.x, trs->localScale.y, trs->localScale.z);

                // Begin transformation
                glm::mat4 transform(1.0f);
                transform = glm::translate(transform, glm::vec3(trs->localPosition.x, trs->localPosition.y, trs->localPosition.z));
                transform = glm::rotate(transform, glm::radians(trs->localRotation.x), { 1,0,0 });
                transform = glm::rotate(transform, glm::radians(trs->localRotation.y), { 0,1,0 });
                transform = glm::rotate(transform, glm::radians(trs->localRotation.z), { 0,0,1 });
                transform = glm::scale(transform, scale);

                EntityId parent = entityManager.GetEntityParent(eid);

                if (parent != eid) {
                    Wiwa::Transform3D* parent_trns = entityManager.GetComponent<Wiwa::Transform3D>(parent);

                    glm::mat4 parent_transform(1.0f);
                    parent_transform = glm::translate(parent_transform, glm::vec3(parent_trns->position.x, parent_trns->position.y, parent_trns->position.z));
                    parent_transform = glm::rotate(parent_transform, glm::radians(parent_trns->rotation.x), glm::vec3(1, 0, 0));
                    parent_transform = glm::rotate(parent_transform, glm::radians(parent_trns->rotation.y), glm::vec3(0, 1, 0));
                    parent_transform = glm::rotate(parent_transform, glm::radians(parent_trns->rotation.z), glm::vec3(0, 0, 1));
                    parent_transform = glm::scale(parent_transform, glm::vec3(parent_trns->scale.x, parent_trns->scale.y, parent_trns->scale.z));

                    transform = parent_transform * transform;

                    scale = glm::vec3(trs->scale.x, trs->scale.y, trs->scale.z);
                }

                // End transformation

                float intersectDist = 0.0f;
                Wiwa::Math::AABB AABB = model->boundingBox;
                //AABB.scale(scale, AABB.getCenter());

                /*if (!m_Camera->frustrum.IsBoxVisible(AABB.getMin(), AABB.getMax()))
                    continue;*/

                if (Wiwa::Math::TestRayOBBIntersection(
                    out_origin,
                    out_dir,
                    AABB.getMin(),
                    AABB.getMax(),
                    transform,
                    intersectDist
                ))
                {
                    /*if (i == 0)
                    {
                        minDist = intersectDist;
                        id = i;
                    }
                    else */if (intersectDist < minDist)
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


