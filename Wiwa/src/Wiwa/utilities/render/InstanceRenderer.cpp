#include <wipch.h>

#include "InstanceRenderer.h"

#include <Wiwa/core/Resources.h>
#include <Wiwa/core/Renderer2D.h>

#include <Wiwa/core/Application.h>

#include <glew.h>

namespace Wiwa {
	InstanceRenderer::InstanceRenderer(uint32_t maxInstances) : m_MaxInstances(maxInstances)
	{
	}

	InstanceRenderer::~InstanceRenderer()
	{

	}

	void InstanceRenderer::Init(const char* shader_path)
	{
		Application& app = Application::Get();

		// Load shader
		// TODO: Use shader pipeline
		//m_InstanceShaderId = Resources::Load<Shader>(shader_path);
		//m_InstanceShader = new Shader();//Resources::GetResourceById<Shader>(m_InstanceShaderId);
		m_InstanceShader.Init(shader_path);

		m_InstanceShader.Bind();
		uint32_t texs_id = glGetUniformLocation(m_InstanceShader.getID(), "u_Textures");

		// Set samplers id
		int samplers[MAX_INSTANCE_TEXTURES];
		for (int i = 0; i < MAX_INSTANCE_TEXTURES; i++) samplers[i] = i;
		glUniform1iv(texs_id, MAX_INSTANCE_TEXTURES, samplers);

		// Vertex quad	
		float verticesInstancedTexture[] = {
			 0.5f,  0.5f, 0.0f, // Top right -> 1.0f, 1.0f
			 0.5f, -0.5f, 0.0f, // Bottom right -> 1.0f, 0.0f
			-0.5f, -0.5f, 0.0f, // Bottom left -> 0.0f, 0.0f
			-0.5f,  0.5f, 0.0f  // Top left -> 0.0f, 1.0f
		};

		// Create buffers
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
		glGenVertexArrays(1, &m_VAO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesInstancedTexture), verticesInstancedTexture, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)0);

		/*glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTexture), (void*)(3 * sizeof(float)));*/

		unsigned int indexes[] = {
			0, 1, 3, 1, 2, 3
		};

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

		glGenBuffers(1, &m_IVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_IVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInstanceTexture) * m_MaxInstances, nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInstanceTexture), (void*)0);
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInstanceTexture), (void*)(2 * sizeof(float)));
		glVertexAttribDivisor(3, 1);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexInstanceTexture), (void*)(4 * sizeof(float)));
		glVertexAttribDivisor(4, 1);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(VertexInstanceTexture), (void*)(8 * sizeof(float)));
		glVertexAttribDivisor(5, 1);

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInstanceTexture), (void*)(9 * sizeof(float)));
		glVertexAttribDivisor(6, 1);

		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInstanceTexture), (void*)(11 * sizeof(float)));
		glVertexAttribDivisor(7, 1);

		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInstanceTexture), (void*)(13 * sizeof(float)));
		glVertexAttribDivisor(8, 1);

		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInstanceTexture), (void*)(15 * sizeof(float)));
		glVertexAttribDivisor(9, 1);

		m_InstanceVertex = new VertexInstanceTexture[m_MaxInstances];

		m_OrthoLocation = m_InstanceShader.getUniformLocation("u_Proj");
		m_ViewLocation = m_InstanceShader.getUniformLocation("u_View");

		m_InstanceCount = 0;

		m_InstanceShader.UnBind();
	}

	void InstanceRenderer::Update()
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_IVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexInstanceTexture) * m_InstanceCount, m_InstanceVertex);
	}

	void InstanceRenderer::Render(glm::mat4& proj, glm::mat4& view)
	{
		m_InstanceShader.Bind();
		m_InstanceShader.setUniform(m_OrthoLocation, proj);
		m_InstanceShader.setUniform(m_ViewLocation, view);

		GLuint texSize = static_cast<GLuint>(m_Textures.size());

		glBindVertexArray(m_VAO);

		for (GLuint i = 0; i < texSize; i++) {
			glBindTextureUnit(i, m_Textures[i]);
		}

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_InstanceCount);

		m_InstanceShader.UnBind();
	}

	uint32_t InstanceRenderer::AddInstance(uint32_t textureId, const Vector2i& position, const Size2i& size, const Color4f& color, const TextureClip& clip, Renderer2D::Pivot pivot)
	{
		if (m_InstanceCount == m_MaxInstances)
			return -1;

		m_InstanceVertex[m_InstanceCount].position = Renderer2D::CalculateScreenGlPos(position, size, pivot);
		m_InstanceVertex[m_InstanceCount].scale = { static_cast<float>(size.x), static_cast<float>(size.y) };
		m_InstanceVertex[m_InstanceCount].color = color;

		int texid = AddTexture(textureId);

		m_InstanceVertex[m_InstanceCount].textureId = static_cast<float>(texid);

		m_InstanceVertex[m_InstanceCount].textureClip = clip;

		return m_InstanceCount++;
	}

	void InstanceRenderer::UpdateInstance(uint32_t id, const Vector2i& position, const  Size2i& size, const Color4f& color, Renderer2D::Pivot pivot)
	{
		m_InstanceVertex[id].position = Renderer2D::CalculateScreenGlPos(position, size, pivot);
		m_InstanceVertex[id].scale = { static_cast<float>(size.x), static_cast<float>(size.y) };
		m_InstanceVertex[id].color = color;
	}

	void InstanceRenderer::UpdateInstance(uint32_t id, const Vector2i& position, Renderer2D::Pivot pivot)
	{
		Vector2i s = { (int)m_InstanceVertex[id].scale.w, (int)m_InstanceVertex->scale.h };
		m_InstanceVertex[id].position = Renderer2D::CalculateScreenGlPos(position, s, pivot);
	}

	int InstanceRenderer::AddTexture(uint32_t texture)
	{
		size_t texSize = m_Textures.size();

		if (texSize > MAX_INSTANCE_TEXTURES) {
			WI_ASSERT_MSG("Trying to add more textures than MAX_INSTANCE_TEXTURES.\n");
		}

		int index = static_cast<int>(texSize);

		for (size_t i = 0; i < texSize; i++) {
			if (m_Textures[i] == texture) {
				index = static_cast<int>(i);
				break;
			}
		}

		if (index == texSize) {
			m_Textures.push_back(texture);
		}

		return index;
	}
}