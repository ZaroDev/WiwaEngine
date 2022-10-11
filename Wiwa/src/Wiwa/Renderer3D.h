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
#include <Wiwa/utilities/render/Mesh.h>

namespace Wiwa {
	class WI_API Renderer3D {
	public:

	private:
		// Default FrameBuffer
		FrameBuffer m_FrameBuffer;

		// Color shader
		ResourceId m_ColorShaderId;
		Shader* m_ColorShader;

		uint32_t m_CSColorUniformLocation;
		uint32_t m_CSModelUniformLocation;
		uint32_t m_CSViewUniformLocation;
		uint32_t m_CSProjectionUniformLocation;

		glm::mat4 m_PersProj;
		glm::mat4 m_View;
		Camera m_ActiveCamera;
	public:
		Renderer3D();
		~Renderer3D();

		bool Init();
		void Update();

		void RenderMeshColor(Mesh& mesh, Vector3f& position, Vector3f& rotation, Vector3f& scale, Color4f& color, FrameBuffer* target=NULL);

		void Close();

		// Getters
		uint32_t getColorBufferTexture() { return m_FrameBuffer.getColorBufferTexture(); }

		glm::mat4 GetPersProjection() { return m_ActiveCamera.getProjection(); }
		glm::mat4 GetView() { return m_ActiveCamera.getView(); }
	};
}