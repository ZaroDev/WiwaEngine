#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/render/Camera.h>
#include <vector>
typedef size_t CameraId;
namespace Wiwa{
	class WI_API CameraManager {
	private:
		static std::vector<Camera*> m_Cameras;
		static CameraId m_ActiveCamera;

		CameraManager() = delete;
	public:
		static void Update();
		static void CleanUp();

		static void SetCamera(CameraId cameraId) { m_ActiveCamera = cameraId; }

		static CameraId getActiveCameraId() { return m_ActiveCamera; }
		static Camera* getActiveCamera() { return m_Cameras[m_ActiveCamera]; }

		inline static size_t getCameraSize() { return m_Cameras.size(); }

		static size_t CreateCamera();
		static size_t CreatePerspectiveCamera(const float fov, const float aspectRatio, const float nearPlaneDistance = 0.1f, const float farPlaneDistance = 1000.0f);
		static Camera* getCamera(CameraId cameraId) { return m_Cameras[cameraId]; }
		inline static std::vector<Camera*>& getScenes() { return m_Cameras; }
	};

}