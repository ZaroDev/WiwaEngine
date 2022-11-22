#include "wipch.h"
#include "CameraManager.h"
namespace Wiwa {
	std::vector<Camera*> CameraManager::m_Cameras;
	CameraId CameraManager::m_ActiveCamera = -1;
	void CameraManager::Update()
	{
		for (size_t i = 0; i < m_Cameras.size(); i++)
			m_Cameras[i]->frameBuffer->Clear();
	}
	void CameraManager::CleanUp()
	{
		for (size_t i = 0; i < m_Cameras.size(); i++)
			delete m_Cameras[i];

		m_Cameras.clear();
	}
	size_t CameraManager::CreateCamera()
	{
		CameraId cameraId = m_Cameras.size();

		Camera* cam = new Camera();

		m_Cameras.push_back(cam);

		return cameraId;
	}
	size_t CameraManager::CreatePerspectiveCamera(const float fov, const float aspectRatio, const float nearPlaneDistance, const float farPlaneDistance)
	{
		CameraId cameraId = CreateCamera();

		Camera* cam = getCamera(cameraId);
		cam->SetPerspective(fov, aspectRatio, nearPlaneDistance, farPlaneDistance);
		
		return cameraId;
	}
}
