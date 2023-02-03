#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/render/RenderLayer.h>
#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/shaders/Shader.h>

#include <glm/mat4x4.hpp>

#include <vector>

#define MAX_LAYERS 10

namespace Wiwa {
	class WI_API RenderManager {
	private:
		RenderManager();

		static FrameBuffer m_FrameBuffer;
		static glm::mat4 m_OrthoProj;
		static glm::mat4 m_View;
		static glm::mat4 m_Model;

		static RenderLayer m_RenderLayers[MAX_LAYERS];

		static Shader m_Shader;

		static uint32_t m_VAO;
		static uint32_t m_VBO;
		static uint32_t m_EBO;

		static uint32_t m_OrthoLoc;
		static uint32_t m_ViewLoc;
		static uint32_t m_ModelLoc;
	public:
		// Initialize render manager
		static void Init(int width, int height);

		// Set camera to specific layer
		static void SetLayerCamera(uint32_t layer_id, Camera* cam) { m_RenderLayers[layer_id].setCamera(cam); }

		// Render all layers into one framebuffer
		static void Update();

		// Get color texture
		static uint32_t getColorTexture() { return m_FrameBuffer.getColorBufferTexture(); }

		// Destroy render manager
		static void Destroy();
	};
}