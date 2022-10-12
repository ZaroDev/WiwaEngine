#include "wipch.h"

#include "MeshViewPanel.h"
#include "../../ShadingView.h"

#include <imgui.h>

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/Application.h>

#include <Wiwa/Renderer2D.h>
#include <Wiwa/Renderer3D.h>

#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/utilities/render/Mesh.h>

#include <Wiwa/utilities/render/primitives/Cube.h>
#include <Wiwa/utilities/render/primitives/Pyramid.h>
#include <Wiwa/utilities/render/primitives/Sphere.h>
#include <Wiwa/utilities/render/primitives/Plane.h>

MeshViewPanel::MeshViewPanel()
    : Panel("Mesh view")
{
    m_FrameBuffer = std::make_unique<Wiwa::FrameBuffer>();
    m_Camera = std::make_unique<Wiwa::Camera>();

    Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
    float ar = res.w / (float)res.h;

    m_FrameBuffer.get()->Init(res.w, res.h);

    m_Camera.get()->SetPerspective(60.0f, ar);
    m_Camera.get()->setPosition({ 0.0f, 1.0f, -5.0f });

    m_ActiveMesh = new Wiwa::Mesh("resources/meshes/cube.fbx");

    m_MeshPosition = { 0.0f, 0.0f, 0.0f };
    m_MeshRotation = {};
    m_MeshScale = { 1.0f, 1.0f, 1.0f };
    
    m_MeshColor = { 1.0f, 1.0f, 1.0f, 1.0f };

    m_Camera.get()->lookat(m_MeshPosition);
}

MeshViewPanel::~MeshViewPanel()
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

    ImTextureID tex = (ImTextureID)(intptr_t)m_FrameBuffer.get()->getColorBufferTexture();

    // Horizontal-align viewport
    ImVec2 cpos = ImGui::GetCursorPos();
    cpos.x = (viewportPanelSize.x - isize.x) / 2;
    ImGui::SetCursorPos(cpos);
    
    // Update mesh rotation with mouse
    ImVec2 mpos = ImGui::GetMousePos();
    ImVec2 cspos = ImGui::GetCursorScreenPos();

    ImVec2 rpos = { mpos.x - cspos.x, mpos.y - cspos.y };
    CLAMP(rpos.x, 0.0f, isize.x);
    CLAMP(rpos.y, 0.0f, isize.y);

    // Render to frame buffer and imgui viewport
    Wiwa::Application::Get().GetRenderer3D().RenderMeshColor(*m_ActiveMesh, m_MeshPosition, m_MeshRotation, m_MeshScale, m_MeshColor, m_FrameBuffer.get(), m_Camera.get());
    ImGui::Image(tex, isize, ImVec2(0, 1), ImVec2(1, 0));

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
                m_ActiveMesh = new Wiwa::Mesh(pathS.c_str());
            }
        }
    
        ImGui::EndDragDropTarget();
    }
    ImGui::End();
}
