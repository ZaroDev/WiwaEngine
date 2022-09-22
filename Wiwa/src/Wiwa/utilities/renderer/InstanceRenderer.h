#pragma once

#include "Empathy/Core.h"
#include "Empathy/simple_types.h"

#include "../Color.h"
#include "Empathy/headers/math/Vector2i.h"

#include "../VertexBuffer.h"

#include "Empathy/headers/Resources.h"
#include "Empathy/headers/Renderer2D.h"

namespace EM {
	class EM_API InstanceRenderer {
		private:
			ResourceId m_InstanceShaderId;
			Shader* m_InstanceShader;

			std::vector<int> m_Textures;

			VertexInstanceTexture* m_InstanceVertex;

			uint32 m_VAO, m_VBO, m_EBO;
			uint32 m_IVBO;

			uint32 m_InstanceCount;
			uint32 m_MaxInstances;

			uint32 m_OrthoLocation;
			uint32 m_ViewLocation;
		public:
			InstanceRenderer(uint32 maxInstances);
			~InstanceRenderer();

			void Init();
			void Update();
			void Render();

			uint32 AddInstance(uint32 textureId, Vector2i& position, Size2i& size, Color4f& color, TextureClip& clip, Renderer2D::Pivot pivot = Renderer2D::Pivot::CENTER);

			void UpdateInstance(uint32 id, Vector2i& position, Size2i& size, Color4f& color, Renderer2D::Pivot pivot = Renderer2D::Pivot::CENTER);
			void UpdateInstance(uint32 id, Vector2i& position, Renderer2D::Pivot pivot = Renderer2D::Pivot::CENTER);
			int AddTexture(uint32 texture);
	};
}