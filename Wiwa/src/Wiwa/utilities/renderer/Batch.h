#pragma once

#include "Empathy/simple_types.h"
#include "../Shader.h"
#include "Empathy/headers/Resources.h"
#include "../VertexBuffer.h"
#include "../Color.h"

namespace EM {
	class Batch {
	private:
		uint32 m_VAO, m_VBO, m_EBO;
		uint32 m_MaxQuads;
		uint32 m_MaxIndices;
		uint32 m_UsedQuads;

		ResourceId m_BatchingShaderId;
		Shader* m_BatchingShader;

		uint32 m_ColorLocation;
		uint32 m_ModelLocation;

		Color m_Color;

		glm::vec4 m_gl_Color;
	public:
		Batch(uint32 max_quads);
		~Batch();

		void SetColor(Color color);

		void AddQuads(Vertex* vertices, uint32 count);
		void Render();
		void Clear();
	};
}