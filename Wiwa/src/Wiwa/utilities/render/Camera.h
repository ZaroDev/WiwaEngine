#pragma once

#include <Wiwa/Core.h>

#include <glm.hpp>
#include <Wiwa/utilities/math/Math.h>

namespace Wiwa {
	class WI_API Camera
	{
	public:
		enum class CameraType {
			INVALID,
			ORTHOGRAPHIC,
			PERSPECTIVE
		};
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

		void setAspectRatio(float aspectRatio);
		float getAspectRatio() { return m_AspectRatio; }

		void setResolution(int width, int height);

		void setFOV(float fov);
		float getFOV() { return m_FOV; }

		void setPosition(Vector3f position);
		void setFront(Vector3f front);

		glm::vec3 getPosition() { return m_CameraPos; }
		glm::vec3 getFront() { return m_CameraFront; }
		glm::vec3 getUp() { return m_CameraUp; }

		void lookat(Vector3f position);

		void SetPerspective(float fov, float aspectRatio, float nearPlaneDistance=0.1f, float farPlaneDistance=100.0f);
		void SetOrthographic(int width, int height, float nearPlaneDistance=0.1f, float farPlaneDistance=100.0f);

		CameraType GetCameraType() { return m_CameraType; }

		glm::mat4 getView() { return m_View; }
		glm::mat4 getProjection() { return m_Projection; }
	};
}