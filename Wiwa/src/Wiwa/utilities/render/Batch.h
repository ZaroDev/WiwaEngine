#pragma once

#include <Wiwa/Resources.h>

#include <Wiwa/utilities/math/Vector4f.h>

#include "VertexBuffer.h"

#define MAX_BATCH_TEXTURES 16

namespace Wiwa {
	class Batch {
	private:
		uint32_t m_VAO, m_VBO, m_EBO;
		uint32_t m_MaxQuads;
		uint32_t m_MaxIndices;
		uint32_t m_UsedQuads;

		ResourceId m_BatchingShaderId;
		Shader* m_BatchingShader;

		uint32_t m_ColorLocation;
		uint32_t m_ModelLocation;

		Color4f m_Color;
	public:
		Batch(uint32_t max_vertices);
		~Batch();

		void SetColor(Color4f color);

		void AddModel(Vertex* vertices, uint32_t count);
		void Render();
		void Clear();
	};
}