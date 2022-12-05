#include "CamerasPanel.h"
#include <Wiwa/utilities/render/CameraManager.h>
#include "../../Utils/ImGuiWidgets.h"
#include <glm/gtc/type_ptr.hpp>

CamerasPanel::CamerasPanel(EditorLayer* instance)
	: Panel("Cameras", instance)
{
}

CamerasPanel::~CamerasPanel()
{
}

void CamerasPanel::Draw()
{
	ImGui::Begin(name, &active);
	size_t cameraCount = Wiwa::CameraManager::getCameraSize();
	if (ImGui::BeginTable("##cameras", 3, ImGuiTableFlags_Resizable))
	{
		ImGui::TableSetupColumn("Camera Id", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Cull", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Transform", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();
		for (size_t row = 0; row < cameraCount; row++)
		{
			Wiwa::Camera* cam = Wiwa::CameraManager::getCamera(row);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%i", row);
			ImGui::TableSetColumnIndex(1);
			ImGui::PushID(row);
			ImGui::Checkbox("Cull##culling", &cam->cull);
			ImGui::Checkbox("BoudingBoxes##culling", &cam->drawBoundingBoxes);
			ImGui::TableSetColumnIndex(2);
			glm::vec3 pos = cam->getPosition();
			glm::vec3 front = cam->getFront();
			DrawVec3Control("Position", pos, 0.0f, 50.0f);
			DrawVec3Control("Front", front, 0.0f, 50.0f);
			cam->setPosition({ pos.x, pos.y, pos.z });
			ImGui::PopID();

		}
		
		ImGui::EndTable();
	}
	ImGui::End();
}

void CamerasPanel::Update()
{
}
