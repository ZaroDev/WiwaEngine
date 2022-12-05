#include "wipch.h"
#include "CameraManager.h"
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Renderer3D.h>
namespace Wiwa {
	std::vector<Camera*> CameraManager::m_Cameras;
	CameraId CameraManager::m_ActiveCamera = -1;
	Camera* CameraManager::editorCamera = nullptr;
	void CameraManager::Init()
	{
		editorCamera = new Camera();
	}
	void CameraManager::Update()
	{
		editorCamera->frameBuffer->Clear();
		if (editorCamera->drawFrustrums)
		{
			Wiwa::Application::Get().GetRenderer3D().RenderFrustrums(editorCamera);
		}
		size_t cameraCount = m_Cameras.size();
		for (size_t i = 0; i < cameraCount; i++)
		{
			m_Cameras[i]->frameBuffer->Clear();
		}
	}
	void CameraManager::CleanUp()
	{
		for (size_t i = 0; i < m_Cameras.size(); i++)
			delete m_Cameras[i];

		m_Cameras.clear();
		delete editorCamera;
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
