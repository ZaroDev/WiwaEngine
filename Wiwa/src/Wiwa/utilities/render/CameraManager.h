#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/render/Camera.h>
#include <vector>

typedef size_t CameraId;

namespace Wiwa{
	class WI_API CameraManager {
	private:
		static std::vector<Camera*> m_Cameras;

		static std::vector<CameraId> m_CamerasAlive;
		static std::vector<CameraId> m_RemovedCameras;

		static CameraId m_ActiveCamera;
		
		CameraManager() = delete;
	public:
		static Camera* editorCamera;
		static void Init();
		static void Update();
		static void Clear();
		static void CleanUp();

		static void setActiveCamera(CameraId cameraId) { m_ActiveCamera = cameraId; }

		static CameraId getActiveCameraId() { return m_ActiveCamera; }
		static Camera* getActiveCamera() { return m_Cameras[m_ActiveCamera]; }

		static size_t getCameraSize() { return m_CamerasAlive.size(); }

		static size_t CreateCamera();
		static size_t CreatePerspectiveCamera(const float fov, const float aspectRatio, const float nearPlaneDistance = 0.1f, const float farPlaneDistance = 100.0f);
		static void DestroyCamera(CameraId cameraId);
		static Camera* getCamera(CameraId cameraId) { return m_Cameras[cameraId]; }
		static std::vector<CameraId>& getCameras() { return m_CamerasAlive; }
	};

}