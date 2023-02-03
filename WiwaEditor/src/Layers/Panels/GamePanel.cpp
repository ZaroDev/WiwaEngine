#include "GamePanel.h"
#include <Wiwa/core/Application.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Renderer3D.h>
#include "../../Utils/EditorUtils.h"

#include <Wiwa/render/RenderManager.h>
#include <Wiwa/core/Renderer2D.h>

GamePanel::GamePanel(EditorLayer* instance)
	: Panel("Game", ICON_FK_GAMEPAD,instance)
{
}

GamePanel::~GamePanel()
{
}

void GamePanel::Draw()
{
    Wiwa::CameraManager& cameraManager = Wiwa::SceneManager::getActiveScene()->GetCameraManager();

    ImGui::Begin(iconName.c_str(), &active);

    ImGui::Checkbox("Show stats", &m_ShowStats);
    ImGui::Separator();


    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    Wiwa::Size2i resolution = Wiwa::Application::Get().GetTargetResolution();
    float ar = resolution.w / (float)resolution.h;
    Wiwa::Size2f scales = { viewportPanelSize.x / (float)resolution.w, viewportPanelSize.y / (float)resolution.h };

    float scale = scales.x < scales.y ? scales.x : scales.y;
    ImVec2 isize = { resolution.w * scale, resolution.h * scale };

    if (cameraManager.getCameraSize() > 0)
    {
        uint32_t cbt = Wiwa::Application::Get().GetRenderer2D().getColorBufferTexture();

        ImTextureID tex = (ImTextureID)(intptr_t)Wiwa::RenderManager::getColorTexture();

        ImVec2 cpos = ImGui::GetCursorPos();
        cpos.x = (viewportPanelSize.x - isize.x) / 2;
        ImGui::SetCursorPos(cpos);
        ImGui::Image(tex, isize, ImVec2(0, 1), ImVec2(1, 0));

        ImVec2 rectPos = ImGui::GetItemRectMin();
        //FPS widget

        if (m_ShowStats)
        {
            ImVec2 rectSize(rectPos.x + 200.0f, rectPos.y + 90.0f);
            ImGui::GetWindowDrawList()->AddRectFilled(
                ImVec2(rectPos.x + 10, rectPos.y),
                rectSize,
                IM_COL32(30, 30, 30, 128)
            );

            ImGui::GetWindowDrawList()->AddRect(
                ImVec2(rectPos.x + 10, rectPos.y),
                rectSize,
                IM_COL32(255, 255, 255, 30)
            );
            float y = cpos.y + 5.0f;
            float x = cpos.x + 15.0f;
            ImGui::SetCursorPos(ImVec2(x, y));
            ImGui::TextColored(ImColor(255, 255, 255, 128), "FPS");
            ImGui::SetCursorPos(ImVec2(x + 60.0f, y));
            ImGui::TextColored(ImColor(255, 255, 255, 128), "%.f FPS", 1000 / Wiwa::Time::GetDeltaTime());

            ImGui::SetCursorPos(ImVec2(x, y + 20.0f));
            ImGui::TextColored(ImColor(255, 255, 255, 128), "Frame time");
            ImGui::SetCursorPos(ImVec2(x + 100.0f, y + 20.0f));
            ImGui::TextColored(ImColor(255, 255, 255, 128), "%.3f ms", Wiwa::Time::GetDeltaTime());

            ImGui::SetCursorPos(ImVec2(x, y + 40.0f));
            ImGui::TextColored(ImColor(255, 255, 255, 128), "Play time");
            ImGui::SetCursorPos(ImVec2(x + 100.0f, y + 40.0f));
            ImGui::TextColored(ImColor(255, 255, 255, 128), "%.3f s", Wiwa::Time::GetTime());

            ImGui::SetCursorPos(ImVec2(x, y + 60.0f));
            ImGui::TextColored(ImColor(255, 255, 255, 128), "Time scale");
            ImGui::SetCursorPos(ImVec2(x + 100.0f, y + 60.0f));
            ImGui::TextColored(ImColor(255, 255, 255, 128), "%.2f", Wiwa::Time::GetTimeScale());
        }
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
