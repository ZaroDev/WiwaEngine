#include <wipch.h>
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <Wiwa/core/Application.h>
#include <glew.h>
#include <gl/GL.h>

namespace Wiwa {
	const int IND_COUNT = 24;

	Camera::Camera()
	{
		// Initialize camera transform
		m_CameraPos = glm::vec3(0.0f, 0.0f, 1.5f);
		m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraRot = glm::vec3(0.0f, 0.0f, 0.0f);

		// Initialize camera distance planes
		m_NearPlaneDist = 0.1f;
		m_FarPlaneDist = 1000.0f;

		// Initialize camera view
		//m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		updateView();

		// Start camera as INVALID
		m_CameraType = CameraType::INVALID;

		int indicies[IND_COUNT] = {
			0, 1, 1, 2, 2, 3, 3, 0, // Front
			4, 5, 5, 6, 6, 7, 7, 4, // Back
			0, 4, 1, 5, 2, 6, 3, 7
		};

		// Generate frustum opengl buffers
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);

		// Dynamic VAO to update frustum vertices
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_FrustumPoints), nullptr, GL_DYNAMIC_DRAW);

		// Static indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	Camera::~Camera()
	{
		delete frameBuffer;
	}

	void Camera::updateView()
	{
		m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
		UpdateFrustrum();
	}

	void Camera::setPlanes(const float nearPlane, const float farPlane)
	{
		m_NearPlaneDist = nearPlane;
		m_FarPlaneDist = farPlane;
		if (m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, nearPlane, farPlane);
		UpdateFrustrum();
	}

	void Camera::setAspectRatio(const float aspectRatio)
	{
		m_AspectRatio = aspectRatio;

		if(m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, m_NearPlaneDist, m_FarPlaneDist);
		UpdateFrustrum();
	}

	void Camera::setResolution(const int width, const int height)
	{
		m_AspectRatio = width / (float)height;

		if (m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlaneDist, m_FarPlaneDist);
		else if(m_CameraType == CameraType::ORTHOGRAPHIC)
			m_Projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
		UpdateFrustrum();
	}

	void Camera::setFOV(const float fov)
	{
		m_FOV = fov;

		if (m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(fov), m_AspectRatio, m_NearPlaneDist, m_FarPlaneDist);
		UpdateFrustrum();
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
		UpdateFrustrum();
	}

	void Camera::lookat(const Vector3f cameraPos, const Vector3f position, const Vector3f camUp)
	{
		glm::vec3 pos = { position.x, position.y, position.z };

		m_CameraPos = { cameraPos.x, cameraPos.y, cameraPos.z };
		m_CameraUp = { camUp.x, camUp.y, camUp.z };
		m_CameraFront = pos - m_CameraPos;

		m_View = glm::lookAt(m_CameraPos, pos, m_CameraUp);
		UpdateFrustrum();
	}

	

	void Camera::SetPerspective(const float fov, const float aspectRatio, const float nearPlaneDistance, const float farPlaneDistance)
	{
		m_CameraType = CameraType::PERSPECTIVE;
		m_FOV = fov;
		m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, nearPlaneDistance, farPlaneDistance);

		m_AspectRatio = aspectRatio;
		m_NearPlaneDist = nearPlaneDistance;
		m_FarPlaneDist = farPlaneDistance;
		UpdateFrustrum();
		Size2i res = Application::Get().GetTargetResolution();
		frameBuffer = new FrameBuffer();
		frameBuffer->Init(res.w, res.h);
		
	}

	void Camera::UpdateFrustrum()
	{
		frustrum = Frustum(m_Projection * m_View);
	}

	void Camera::SetOrthographic(const int width, const int height, const float nearPlaneDistance, const float farPlaneDistance)
	{
		m_CameraType = CameraType::ORTHOGRAPHIC;

		m_Projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, nearPlaneDistance, farPlaneDistance);

		m_AspectRatio = width / (float)height;
		m_NearPlaneDist = nearPlaneDistance;
		m_FarPlaneDist = farPlaneDistance;
	}
	void Camera::DrawFrustrum()
	{
		glLineWidth(5.0f);

		// Generate frustum data
		GetCornerPoints(m_FrustumPoints);

		glBindVertexArray(vao);

		// Update frumstum data
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_FrustumPoints), m_FrustumPoints);

		// Draw frustum
		glDrawElements(GL_LINES, (GLsizei)IND_COUNT, GL_UNSIGNED_INT, 0);

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glLineWidth(1.0f);
		
	}
}