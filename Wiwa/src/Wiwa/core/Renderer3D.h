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

		// Color shader
		ResourceId m_ColorShaderId;
		Shader* m_ColorShader;
		uint32_t m_CSColorUniformLocation;
		DefaultLitUniforms m_CSUniforms;
		DirLightUniforms m_CSDLUniforms;

		ResourceId m_TextureShaderId;
		Shader* m_TextureShader;
		DefaultLitUniforms m_TSUniforms;
		DirLightUniforms m_TSDLUniforms;
		PointLightUniforms m_TSPLUniforms;

		ResourceId m_NormalDisplayShaderId;
		Shader* m_NormalDisplayShader;
		DefaultUnlitUniforms m_NDSUniforms;

		ResourceId m_BBDisplayShaderId;
		Shader* m_BBDisplayShader;
		DefaultUnlitUniforms m_BBDSUniforms;


		glm::mat4 m_PersProj{ 0.0f };
		glm::mat4 m_View{ 0.0f };
		Camera* m_ActiveCamera;

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
		void RenderMeshColor(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, bool clear=false, Camera* camera=NULL , bool cull = false);
		void RenderMeshMaterial(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, bool clear=false, Camera* camera=NULL, bool cull = false);
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