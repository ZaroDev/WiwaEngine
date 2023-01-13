#pragma once

#include <Wiwa/core/Core.h>

#include <Wiwa/utilities/math/Math.h>

#include "VertexBuffer.h"

#include <Wiwa/core/Resources.h>
#include <Wiwa/core/Renderer2D.h>

#define MAX_INSTANCE_TEXTURES 16

namespace Wiwa {
	class WI_API InstanceRenderer {
		private:
			ResourceId m_InstanceShaderId;
			Shader* m_InstanceShader;

			std::vector<int> m_Textures;

			VertexInstanceTexture* m_InstanceVertex;

			uint32_t m_VAO, m_VBO, m_EBO;
			uint32_t m_IVBO;

			uint32_t m_InstanceCount;
			uint32_t m_MaxInstances;

			uint32_t m_OrthoLocation;
			uint32_t m_ViewLocation;
		public:
			InstanceRenderer(uint32_t maxInstances);
			~InstanceRenderer();

			void Init(const char* shader_path);
			void Update();
			void Render();

			uint32_t AddInstance(uint32_t textureId, const Vector2i& position, const Size2i& size, const Color4f& color, const TextureClip& clip, Renderer2D::Pivot pivot = Renderer2D::Pivot::CENTER);

			void UpdateInstance(uint32_t id, const Vector2i& position, const Size2i& size, const Color4f& color, Renderer2D::Pivot pivot = Renderer2D::Pivot::CENTER);
			void UpdateInstance(uint32_t id, const Vector2i& position, Renderer2D::Pivot pivot = Renderer2D::Pivot::CENTER);
			int AddTexture(uint32_t texture);
	};
}