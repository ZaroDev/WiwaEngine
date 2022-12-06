#include "CamerasPanel.h"
#include <Wiwa/utilities/render/CameraManager.h>
#include "../../Utils/ImGuiWidgets.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Wiwa/core/Application.h>
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
	Wiwa::Camera* editorCam = Wiwa::CameraManager::editorCamera;
	if (ImGui::Button("Create Camera"))
	{
		Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
		float ar = res.w / (float)res.h;
		Wiwa::CameraManager::CreatePerspectiveCamera(60, ar);
	}
	ImGui::SameLine();
	ImGui::Checkbox("Show AABB", &editorCam->drawBoundingBoxes);
	ImGui::SameLine();
	ImGui::Checkbox("Show Frustrums", &editorCam->drawFrustrums);
	size_t cameraCount = Wiwa::CameraManager::getCameraSize();
	if (ImGui::BeginTable("##cameras", 3, ImGuiTableFlags_Resizable))
	{
		ImGui::TableSetupColumn("Camera Id", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Options", ImGuiTableColumnFlags_WidthFixed);
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
			bool selected =
				Wiwa::CameraManager::getActiveCamera() == Wiwa::CameraManager::getCamera(row) ?
				true : false;
			ImGui::Checkbox("Active", &selected);
			if (selected)
			{
				Wiwa::CameraManager::setActiveCamera(row);
			}
			if (ImGui::Button("Delete"))
			{
				Wiwa::CameraManager::DestroyCamera(row);
			}
			ImGui::TableSetColumnIndex(2);
			glm::vec3 pos = cam->getPosition();
			glm::vec3 angles = cam->getRotation();
			
			float fov = cam->getFOV();
			float nearP = cam->getNear();
			float farP = cam->getFar();
			DrawVec3Control("Position", glm::value_ptr(pos), 0.0f, 50.0f);
			DrawVec3Control("Angles", glm::value_ptr(angles), 0.0f, 50.0f);
			cam->setRotation(angles);
			ImGui::DragFloat("Fov", &fov);
			ImGui::DragFloat("Near", &nearP);
			ImGui::DragFloat("Far", &farP);
			cam->setPosition({ pos.x, pos.y, pos.z });
			glm::vec3 direction;
			direction.x = cos(glm::radians(angles.x)) * cos(glm::radians(angles.y));
			direction.y = sin(glm::radians(angles.y));
			direction.z = sin(glm::radians(angles.x)) * cos(glm::radians(angles.y));

			glm::vec3 front = glm::normalize(direction);
			cam->setFront({ front.x, front.y, front.z });
			
			cam->setFOV(fov);
			cam->setPlanes(nearP, farP);
			ImGui::PopID();

		}
		
		ImGui::EndTable();
	}
	ImGui::End();
}

void CamerasPanel::Update()
{
}
