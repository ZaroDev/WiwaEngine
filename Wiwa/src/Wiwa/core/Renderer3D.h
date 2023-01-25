#pragma once
#pragma warning( disable : 4251 )

#include <Wiwa/core/Core.h>

// 3D maths lib
//#include <MathGeoLib.h>
// 2D maths lib
#include <Wiwa/utilities/math/Math.h>

#include <Wiwa/utilities/Log.h>
#include <Wiwa/core/Resources.h>

//#include "utilities/gui/Texture.h"

#include <Wiwa/utilities/render/VertexBuffer.h>

//#include "Empathy/headers/utilities/renderer/Batch.h"
#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/ecs/components/Transform3D.h>

#include <Wiwa/utilities/render/Uniforms.h>

namespace Wiwa {
	class WI_API Renderer3D {
	public:
		enum Options
		{
			DEPTH_TEST = 0,
			CULL_FACE,
			LIGHTING,
			COLOR_MATERIAL,
			TEXTURE_2D,
			WIREFRAME
		};
	private:
		glm::mat4 m_PersProj{ 0.0f };
		glm::mat4 m_View{ 0.0f };
		Camera* m_ActiveCamera;


		ResourceId m_BBDisplayShaderId;
		Shader* m_BBDisplayShader;
		DefaultUnlitUniforms m_BBDSUniforms;

		ResourceId m_NormalDisplayShaderId;
		Shader* m_NormalDisplayShader;
		DefaultUnlitUniforms m_NDSUniforms;

	public:
		Renderer3D();
		~Renderer3D();

		bool Init();
		void Update();

		inline void SetActiveCamera(Camera* cam) 
		{
			m_ActiveCamera = cam; 
		}
		void SetOption(Options option);
		void DisableOption(Options option);

		void RenderMesh(Model* mesh, const Transform3D& t3d, Material* material, bool clear=false, Camera* camera=NULL, bool cull = false);
		void RenderMesh(Model* mesh, const Transform3D& t3d, const Transform3D& parent, Material* material, bool clear = false, Camera* camera = NULL, bool cull = false);
		void RenderMesh(Model* mesh, const Vector3f& position, const Vector3f& rotation, const Vector3f& scale, Material* material, bool clear = false, Camera* camera = NULL, bool cull = false);

		void Close();
		void RenderFrustrums(Camera* camera = NULL);
		inline void SetLight(const DirectionalLight& light) { m_ActiveCamera->frameBuffer->setLight(light); }

		// Getters
		inline uint32_t getColorBufferTexture() { return m_ActiveCamera->frameBuffer->getColorBufferTexture(); }

		inline FrameBuffer* getFrameBuffer() { return m_ActiveCamera->frameBuffer; }

		glm::mat4 GetPersProjection() { return m_ActiveCamera->getProjection(); }
		glm::mat4 GetView() { return m_ActiveCamera->getView(); }
	};
}