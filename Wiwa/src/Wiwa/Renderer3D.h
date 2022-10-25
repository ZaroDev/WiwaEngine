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
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>

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
		struct DefaultUnlitUniforms
		{
			uint32_t Projection;
			uint32_t View;
			uint32_t Model;
		};
		struct DefaultLitUniforms
		{
			uint32_t MatAmbient;
			uint32_t MatDiffuse;
			uint32_t MatSpecular;

			uint32_t Shininess;

			uint32_t ViewPos;

			uint32_t LigPos;
			uint32_t LigAmbient;
			uint32_t LigDiffuse;
			uint32_t LigSpecular;
			
			uint32_t Projection;
			uint32_t View;
			uint32_t Model;
		};
		// Default FrameBuffer
		FrameBuffer m_FrameBuffer;

		// Color shader
		ResourceId m_ColorShaderId;
		Shader* m_ColorShader;
		uint32_t m_CSColorUniformLocation;
		DefaultLitUniforms m_CSUniforms;

		ResourceId m_TextureShaderId;
		Shader* m_TextureShader;
		DefaultLitUniforms m_TSUniforms;


		glm::mat4 m_PersProj{ 0.0f };
		glm::mat4 m_View{ 0.0f };
		Camera m_ActiveCamera;

	public:
		Renderer3D();
		~Renderer3D();

		bool Init();
		void Update();

		inline void SetActiveCamera(Camera cam) { m_ActiveCamera = cam; }
		void SetOption(Options option);
		void DisableOption(Options option);
		void RenderMeshColor(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, bool clear=false, FrameBuffer* target=NULL, Camera* camera=NULL);
		void RenderMeshMaterial(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, bool clear=false, FrameBuffer* target=NULL, Camera* camera=NULL);
		void RenderGrid(Model* grid, FrameBuffer* target = NULL, bool clear = false, Camera* camera = NULL);
		void Close();
		
		inline void SetLight(const Light& light) { m_FrameBuffer.setLight(light); }

		// Getters
		uint32_t getColorBufferTexture() { return m_FrameBuffer.getColorBufferTexture(); }

		inline FrameBuffer& getFrameBuffer() { return m_FrameBuffer; }

		glm::mat4 GetPersProjection() { return m_ActiveCamera.getProjection(); }
		glm::mat4 GetView() { return m_ActiveCamera.getView(); }
	};
}