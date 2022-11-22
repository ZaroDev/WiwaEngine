#pragma once

#include <Wiwa/core/Core.h>

#include <glm.hpp>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/math/Frustrum.h>
#include <Wiwa/utilities/render/FrameBuffer.h>
namespace Wiwa {
	class WI_API Camera
	{
	public:
		enum class CameraType {
			INVALID,
			ORTHOGRAPHIC,
			PERSPECTIVE
		};
		Frustum frustrum;
		FrameBuffer* frameBuffer;
		bool cull = false;
	private:
		float m_FOV;
		CameraType m_CameraType;

		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;

		glm::mat4 m_View;
		glm::mat4 m_Projection;

		float m_NearPlaneDist;
		float m_FarPlaneDist;
		float m_AspectRatio;
		void updateView();
	public:
		Camera();
		~Camera();

		void setPlanes(const float nearPlane, const float farPlane);
		void setAspectRatio(const float aspectRatio);
		float getAspectRatio() { return m_AspectRatio; }

		void setResolution(const int width, const int height);

		void setFOV(const float fov);
		float getFOV() { return m_FOV; }

		void setPosition(Vector3f position);
		void setFront(Vector3f front);

		glm::vec3 getPosition() { return m_CameraPos; }
		glm::vec3 getFront() { return m_CameraFront; }
		glm::vec3 getUp() { return m_CameraUp; }

		void lookat(const Vector3f position);
		void lookat(const Vector3f cameraPos, const Vector3f position, const Vector3f camUp);

		void SetPerspective(const float fov, const float aspectRatio, const float nearPlaneDistance=0.1f, const float farPlaneDistance=100.0f);
		void UpdateFrustrum();
		void SetOrthographic(const int width, const int height, const float nearPlaneDistance=0.1f, const float farPlaneDistance=100.0f);

		CameraType GetCameraType() { return m_CameraType; }

		glm::mat4 getView() { return m_View; }
		glm::mat4 getProjection() { return m_Projection; }
		
	};
}