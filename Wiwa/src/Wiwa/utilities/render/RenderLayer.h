#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/render/Camera.h>

namespace Wiwa {
	class WI_API RenderLayer {
		private:
			// Camera associated to this layer
			Camera* m_LayerCamera;
		public:
			// Default constructor
			RenderLayer() : m_LayerCamera(NULL){}

			// Set the associated camera
			void setCamera(Camera* cam) { m_LayerCamera = cam; }

			// Get the associated camera
			Camera* getCamera() { return m_LayerCamera; }

			// Get the framebuffer of the associated camera
			FrameBuffer* getFrameBuffer() { return m_LayerCamera->frameBuffer; }

			// Get the color texture of the associated camera's framebuffer
			uint32_t getColorTexture() { return m_LayerCamera->frameBuffer->getColorBufferTexture(); }
	};
}