#pragma once
#pragma warning( disable : 4251 )

#include <Wiwa/Core.h>

// 3D maths lib
//#include <MathGeoLib.h>
// 2D maths lib
#include <Wiwa/utilities/math/Math.h>

#include <Wiwa/Log.h>
#include <Wiwa/Resources.h>

//#include "utilities/gui/Texture.h"

#include <Wiwa/utilities/render/VertexBuffer.h>

//#include "Empathy/headers/utilities/renderer/Batch.h"
#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Camera.h>

namespace Wiwa {
	class InstanceRenderer;

	class WI_API Renderer3D {
	public:

	private:
		// Default FrameBuffer
		FrameBuffer m_FrameBuffer;

		glm::mat4 m_PersProj;
		glm::mat4 m_View;
		Camera m_ActiveCamera;
	public:
		Renderer3D();
		~Renderer3D();

		bool Init();
		void Update();

		void Close();

		// Getters
		uint32_t getColorBufferTexture() { return m_FrameBuffer.getColorBufferTexture(); }

		glm::mat4 GetPersProjection() { return m_ActiveCamera.getProjection(); }
		glm::mat4 GetView() { return m_ActiveCamera.getView(); }
	};
}