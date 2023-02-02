#include "CamerasPanel.h"
#include <Wiwa/utilities/render/CameraManager.h>
#include "../../Utils/EditorUtils.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Application.h>
CamerasPanel::CamerasPanel(EditorLayer* instance)
	: Panel("Cameras", ICON_FK_CAMERA, instance)
{
}

CamerasPanel::~CamerasPanel()
{
}

void CamerasPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
	Wiwa::CameraManager& cameraManager = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
	Wiwa::Camera* editorCam = cameraManager.editorCamera;
	if (ImGui::Button("Create Camera"))
	{
		Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
		float ar = res.w / (float)res.h;
		cameraManager.CreatePerspectiveCamera(60, ar);
	}
	ImGui::SameLine();
	ImGui::Checkbox("Show AABB", &editorCam->drawBoundingBoxes);
	ImGui::SameLine();
	ImGui::Checkbox("Show Frustrums", &editorCam->drawFrustrums);
	

	std::vector<CameraId>& cameras = cameraManager.getCameras();
	size_t cameraCount = cameraManager.getCameraSize();

	if (ImGui::BeginTable("##cameras", 3, ImGuiTableFlags_Resizable))
	{
		ImGui::TableSetupColumn("Camera Id", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Options", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Transform", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		CameraId remove_cam_id = -1;
		bool remove_cam = false;

		for (size_t row = 0; row < cameraCount; row++)
		{
			CameraId cam_id = cameras[row];
			Wiwa::Camera* cam = cameraManager.getCamera(cam_id);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%i", cam_id);
			ImGui::TableSetColumnIndex(1);
			ImGui::PushID(cam_id);
			ImGui::Checkbox("Cull##culling", &cam->cull);
			ImGui::Checkbox("BoudingBoxes##culling", &cam->drawBoundingBoxes);
			bool selected =
				cameraManager.getActiveCamera() == cameraManager.getCamera(cam_id) ?
				true : false;

			ImGui::Checkbox("Active", &selected);
			if (selected)
			{
				cameraManager.setActiveCamera(cam_id);
			}
			if (ImGui::Button("Delete"))
			{
				remove_cam_id = cam_id;
				remove_cam = true;
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
			
			cam->setFOV(fov);
			cam->setPlanes(nearP, farP);
			ImGui::PopID();

		}

		if (remove_cam) {
			cameraManager.DestroyCamera(remove_cam_id);
		}
		
		ImGui::EndTable();
	}
	ImGui::End();
}

void CamerasPanel::Update()
{
}
