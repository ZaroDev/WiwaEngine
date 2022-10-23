#include "wipch.h"

#include "MeshViewPanel.h"
#include "../../ShadingView.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/Application.h>

#include <Wiwa/Renderer2D.h>
#include <Wiwa/Renderer3D.h>

#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/ecs/components/Mesh.h>

#include <Wiwa/Input.h>
#include <Wiwa/KeyCodes.h>

#include <Wiwa/ecs/EntityManager.h>
#include <gtc/quaternion.hpp>

MeshViewPanel::MeshViewPanel(EditorLayer* instance)
    : Panel("Mesh view", instance)
{
    Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
    float ar = res.w / (float)res.h;

    m_FrameBuffer.Init(res.w, res.h);

    m_Camera.SetPerspective(45.0f, ar);
    m_Camera.setPosition({ 0.0f, 0.0f, 7.0f });

    m_ActiveMesh = new Wiwa::Model("resources/meshes/BakerHouse.fbx");
    m_ActiveMaterial = new Wiwa::Material("assets/textures/test.wimaterial");

    m_MeshPosition = { 0.0f, -1.0f, 0.0f };
    m_MeshRotation = { 0.0f, 0.0f, 0.0f };
    m_MeshScale = { 1.0f, 1.0f, 1.0f };
    
    m_MeshColor = { 1.0f, 1.0f, 1.0f, 1.0f };

    m_Camera.lookat(m_MeshPosition);

    // Camera control
    camSpeed = 0.005f;
    sensitivity = 0.5f;

    yaw = 0.0f;
    pitch = 0.0f;
}

MeshViewPanel::~MeshViewPanel()
{
}

void MeshViewPanel::Update()
{
}

void MeshViewPanel::Draw()
{
    ImGui::Begin(name, &active, ImGuiWindowFlags_MenuBar);
    
    // Calculate viewport aspect ratio
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    Wiwa::Size2i resolution = Wiwa::Application::Get().GetTargetResolution();
    Wiwa::Size2f scales = { viewportPanelSize.x / (float)resolution.w, viewportPanelSize.y / (float)resolution.h };

    float scale = scales.x < scales.y ? scales.x : scales.y;

    ImVec2 isize = { resolution.w * scale, resolution.h * scale };

    ImTextureID tex = (ImTextureID)(intptr_t)m_FrameBuffer.getColorBufferTexture();

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
        if (Wiwa::Input::IsMouseButtonPressed(0))
        {
            // Check if relative motion is not 0
            if (rel2f != Wiwa::Vector2f::Zero()) {
                float xoffset = -rel2f.x * sensitivity;
                float yoffset = rel2f.y * sensitivity;
                pitch += yoffset;
                yaw += xoffset;

                float radius = 7.0f;
                glm::vec3 direction = {};
                direction.x = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw)) + m_MeshPosition.x;
                direction.y = radius * sin(glm::radians(pitch)) * sin(glm::radians(yaw)) + m_MeshPosition.y;
                direction.z = radius * cos(glm::radians(yaw)) + m_MeshPosition.z;
                m_Camera.setPosition({ direction.x, direction.y, direction.z });
                m_Camera.lookat(m_MeshPosition);
            }
        }
        if (Wiwa::Input::IsMouseButtonPressed(1)) {
            // Check if relative motion is not 0
            if (rel2f != Wiwa::Vector2f::Zero()) {
                float xoffset = -rel2f.x * sensitivity;
                float yoffset = rel2f.y * sensitivity;

                yaw += xoffset;
                pitch += yoffset;

                /*if (pitch > 89.0f) pitch = 89.0f;
                if (pitch < -89.0f) pitch = -89.0f;*/

                glm::vec3 direction;
                direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                direction.y = sin(glm::radians(pitch));
                direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

                glm::vec3 front = glm::normalize(direction);
                m_Camera.setFront({ front.x, front.y, front.z });

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
        //FOV
        float fov = m_Camera.getFOV();
        if (m_Scroll > 0)
            fov -= 10;
        else if (m_Scroll < 0)
            fov += 10;

        CLAMP(fov, 1, 120);
        m_Scroll = 0.0f;
        m_Camera.setFOV(fov);
    }

    // Render to frame buffer and imgui viewport
    Wiwa::Application::Get().GetRenderer3D().RenderMeshMaterial(m_ActiveMesh, m_MeshPosition, m_MeshRotation, m_MeshScale, m_ActiveMaterial, true, &m_FrameBuffer, &m_Camera);
    ImGui::Image(tex, isize, ImVec2(0, 1), ImVec2(1, 0));

    ImVec2 rectPos = ImGui::GetItemRectMin();
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
    ImGui::TextColored(ImColor(255, 255, 255, 128), "Camera Pos");
    ImGui::SetCursorPos(ImVec2(x, y + 20.0f));
    ImGui::TextColored(ImColor(255, 255, 255, 128), "%.f x", m_Camera.getPosition().x);
    ImGui::SameLine();
    ImGui::TextColored(ImColor(255, 255, 255, 128), "%.f y", m_Camera.getPosition().y);
    ImGui::SameLine();
    ImGui::TextColored(ImColor(255, 255, 255, 128), "%.f z", m_Camera.getPosition().z);

    ImGui::SetCursorPos(ImVec2(x, y + 30.0f));
    ImGui::TextColored(ImColor(255, 255, 255, 128), "Angles");
    ImGui::SetCursorPos(ImVec2(x, y + 50.0f));
    ImGui::TextColored(ImColor(255, 255, 255, 128), "Pitch: %.3f ", pitch);
    ImGui::SameLine();
    ImGui::TextColored(ImColor(255, 255, 255, 128), "Yaw: %.3f ", yaw);

    //Drag and drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            const wchar_t* path = (const wchar_t*)payload->Data;
            std::wstring ws(path);
            std::string pathS(ws.begin(), ws.end());
            std::filesystem::path p = pathS.c_str();
            if (p.extension() == ".fbx" || p.extension() == ".FBX")
            {
                WI_INFO("Trying to load payload at path {0}", pathS.c_str());
                //TODO: Load the model
                ResourceId id = Wiwa::Resources::Load<Wiwa::Model>(pathS.c_str());
                m_ActiveMesh = Wiwa::Resources::GetResourceById<Wiwa::Model>(id);
                m_Camera.setPosition({ 0.0f, 1.0f, 5.0f });
                m_Camera.lookat({ 0.0f, 0.0f, 0.0f });
            }
            if (p.extension() == ".wimaterial")
            {
                WI_INFO("Trying to load payload at path {0}", pathS.c_str());
                ResourceId id = Wiwa::Resources::Load<Wiwa::Material>(pathS.c_str());
                m_ActiveMaterial = Wiwa::Resources::GetResourceById<Wiwa::Material>(id);
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

void MeshViewPanel::OnEvent(Wiwa::Event& e)
{
    Wiwa::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<EntityChangeEvent>({ &MeshViewPanel::EntityChange, this });
    dispatcher.Dispatch<Wiwa::MouseScrolledEvent>({ &MeshViewPanel::OnMouseScrollEvent, this });
}

bool MeshViewPanel::EntityChange(EntityChangeEvent& e)
{
    Wiwa::Mesh* mesh = Wiwa::Application::Get().GetEntityManager().GetComponent<Wiwa::Mesh>(e.GetResourceId());
    if (!mesh)
        return false;

    m_ActiveMaterial = Wiwa::Resources::GetResourceById<Wiwa::Material>(mesh->materialId);
    m_ActiveMesh = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);

    return false;
}

bool MeshViewPanel::OnMouseScrollEvent(Wiwa::MouseScrolledEvent& e)
{
    m_Scroll = e.GetYOffset();
    return false;
}


