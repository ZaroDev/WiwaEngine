#include <wipch.h>
#include "Camera.h"

#include <gtc/matrix_transform.hpp>

namespace Wiwa {
	Camera::Camera()
	{
		// Initialize camera transform
		m_CameraPos = glm::vec3(0.0f, 0.0f, 1.5f);
		m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// Initialize camera distance planes
		m_NearPlaneDist = 0.1f;
		m_FarPlaneDist = 1000.0f;

		// Initialize camera view
		//m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		updateView();

		// Start camera as INVALID
		m_CameraType = CameraType::INVALID;
	}


	Camera::~Camera()
	{
	}

	void Camera::updateView()
	{
		m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	}

	void Camera::setPlanes(const float nearPlane, const float farPlane)
	{
		m_NearPlaneDist = nearPlane;
		m_FarPlaneDist = farPlane;
		if (m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, nearPlane, farPlane);
	}

	void Camera::setAspectRatio(const float aspectRatio)
	{
		m_AspectRatio = aspectRatio;

		if(m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, m_NearPlaneDist, m_FarPlaneDist);
	}

	void Camera::setResolution(const int width, const int height)
	{
		m_AspectRatio = width / (float)height;

		if (m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlaneDist, m_FarPlaneDist);
		else if(m_CameraType == CameraType::ORTHOGRAPHIC)
			m_Projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
	}

	void Camera::setFOV(const float fov)
	{
		m_FOV = fov;

		if (m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(fov), m_AspectRatio, m_NearPlaneDist, m_FarPlaneDist);
	}

	void Camera::setPosition(const Vector3f position)
	{
		m_CameraPos = glm::vec3(position.x, position.y, position.z);

		updateView();
	}

	void Camera::setFront(const Vector3f front)
	{
		m_CameraFront = glm::vec3(front.x, front.y, front.z);

		updateView();
	}

	void Camera::lookat(const Vector3f position)
	{
		glm::vec3 pos = { position.x, position.y, position.z };

		m_CameraFront = pos - m_CameraPos;

		m_View = glm::lookAt(m_CameraPos, pos, m_CameraUp);
	}

	void Camera::lookat(const Vector3f cameraPos, const Vector3f position, const Vector3f camUp)
	{
		glm::vec3 pos = { position.x, position.y, position.z };

		m_CameraPos = { cameraPos.x, cameraPos.y, cameraPos.z };
		m_CameraUp = { camUp.x, camUp.y, camUp.z };
		m_CameraFront = pos - m_CameraPos;

		m_View = glm::lookAt(m_CameraPos, pos, m_CameraUp);
	}

	void Camera::SetPerspective(const float fov, const float aspectRatio, const float nearPlaneDistance, const float farPlaneDistance)
	{
		m_CameraType = CameraType::PERSPECTIVE;
		m_FOV = fov;
		m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, nearPlaneDistance, farPlaneDistance);

		m_AspectRatio = aspectRatio;
		m_NearPlaneDist = nearPlaneDistance;
		m_FarPlaneDist = farPlaneDistance;
	}

	void Camera::SetOrthographic(const int width, const int height, const float nearPlaneDistance, const float farPlaneDistance)
	{
		m_CameraType = CameraType::ORTHOGRAPHIC;

		m_Projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, nearPlaneDistance, farPlaneDistance);

		m_AspectRatio = width / (float)height;
		m_NearPlaneDist = nearPlaneDistance;
		m_FarPlaneDist = farPlaneDistance;
	}
}