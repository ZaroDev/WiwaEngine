#pragma once

#include <Wiwa/core/Core.h>

#include <glm/glm.hpp>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/math/Frustum.h>
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
		bool drawBoundingBoxes = false;
		bool drawFrustrums = true;
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

		std::vector<int> indicies;
		std::vector<float> vertices;
		unsigned int vao, vbo, ebo;
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
		float getFar() { return m_FarPlaneDist; }
		float getNear() { return m_NearPlaneDist; }

		void lookat(const Vector3f position);
		void lookat(const Vector3f cameraPos, const Vector3f position, const Vector3f camUp);
		void SetPerspective(const float fov, const float aspectRatio, const float nearPlaneDistance=0.1f, const float farPlaneDistance=100.0f);
		void UpdateFrustrum();
		void SetOrthographic(const int width, const int height, const float nearPlaneDistance=0.1f, const float farPlaneDistance=100.0f);

		CameraType GetCameraType() { return m_CameraType; }

		glm::mat4 getView() { return m_View; }
		glm::mat4 getProjection() { return m_Projection; }
		
		void DrawFrustrum();
		inline glm::vec3 FarPlanePos(float x, float y) const
		{
			float farPlaneHalfWidth = glm::tan(m_FOV * 0.5f)* m_FarPlaneDist;
			float farPlaneHalfHeight = glm::tan(m_FOV * 0.5f) * m_FarPlaneDist;
			x = x * farPlaneHalfWidth;
			y = y * farPlaneHalfHeight;
			glm::vec3 right = glm::cross(m_CameraFront, m_CameraUp);
			return m_CameraPos + m_CameraFront * m_FarPlaneDist + x * right + y * m_CameraUp;
		}
		inline glm::vec3 NearPlanePos(float x, float y) const 
		{
			float frontPlaneHalfWidth = glm::tan(m_FOV * 0.5f) * m_NearPlaneDist;
			float frontPlaneHalfHeight = glm::tan(m_FOV * 0.5f) * m_NearPlaneDist;
			x = x * frontPlaneHalfWidth; // Map [-1,1] to [-width/2, width/2].
			y = y * frontPlaneHalfHeight;  // Map [-1,1] to [-height/2, height/2].
			glm::vec3 right = glm::cross(m_CameraFront, m_CameraUp);
			return m_CameraPos + m_CameraFront * m_NearPlaneDist + x * right + y * m_CameraUp;
		}
		inline void GetCornerPoints(glm::vec3* points)
		{
			float tanhfov = glm::tan(m_FOV * 0.5f);
			float tanvfov = glm::tan(m_FOV * 0.5f);
			float frontPlaneHalfWidth = tanhfov * m_NearPlaneDist;
			float frontPlaneHalfHeight = tanvfov * m_NearPlaneDist;
			float farPlaneHalfWidth = tanhfov * m_FarPlaneDist;
			float farPlaneHalfHeight = tanvfov * m_FarPlaneDist;

			glm::vec3 right = glm::cross(m_CameraUp, m_CameraFront);

			glm::vec3 nearCenter = m_CameraPos + m_CameraFront * m_NearPlaneDist;
			glm::vec3 nearHalfWidth = frontPlaneHalfWidth * right;
			glm::vec3 nearHalfHeight = frontPlaneHalfHeight * m_CameraUp;
			points[0] = nearCenter - nearHalfWidth - nearHalfHeight;
			points[1] = nearCenter + nearHalfWidth - nearHalfHeight;
			points[2] = nearCenter - nearHalfWidth + nearHalfHeight;
			points[3] = nearCenter + nearHalfWidth + nearHalfHeight;

			glm::vec3 farCenter = m_CameraPos + m_CameraFront * m_FarPlaneDist;
			glm::vec3 farHalfWidth = farPlaneHalfWidth * right;
			glm::vec3 farHalfHeight = farPlaneHalfHeight * m_CameraUp;
			points[4] = farCenter - farHalfWidth - farHalfHeight;
			points[5] = farCenter + farHalfWidth - farHalfHeight;
			points[6] = farCenter - farHalfWidth + farHalfHeight;
			points[7] = farCenter + farHalfWidth + farHalfHeight;
		}
	};
}