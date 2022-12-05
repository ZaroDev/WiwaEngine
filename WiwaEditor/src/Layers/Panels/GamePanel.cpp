#include "GamePanel.h"
#include <Wiwa/core/Application.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Renderer3D.h>
#include "../../Utils/ImGuiWidgets.h"

GamePanel::GamePanel(EditorLayer* instance)
	: Panel("Game",instance)
{
}

GamePanel::~GamePanel()
{
}

void GamePanel::Draw()
{
    ImGui::Begin(name, &active);
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    Wiwa::Size2i resolution = Wiwa::Application::Get().GetTargetResolution();
    float ar = resolution.w / (float)resolution.h;
    Wiwa::Size2f scales = { viewportPanelSize.x / (float)resolution.w, viewportPanelSize.y / (float)resolution.h };

    float scale = scales.x < scales.y ? scales.x : scales.y;

    ImVec2 isize = { resolution.w * scale, resolution.h * scale };
    if (Wiwa::CameraManager::getCameraSize() > 0)
    {
        Wiwa::Camera* cam = Wiwa::CameraManager::getActiveCamera();

        ImTextureID tex = (ImTextureID)(intptr_t)cam->frameBuffer->getColorBufferTexture();

        ImVec2 cpos = ImGui::GetCursorPos();
        cpos.x = (viewportPanelSize.x - isize.x) / 2;
        ImGui::SetCursorPos(cpos);
        ImGui::Image(tex, isize, ImVec2(0, 1), ImVec2(1, 0));
    }
    else
    {
        TextCentered("No cameras to display, please create a camera to render the scene!");
    }
    ImGui::End();
}

void GamePanel::Update()
{
}
