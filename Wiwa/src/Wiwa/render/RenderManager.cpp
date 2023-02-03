#include <wipch.h>
#include <Wiwa/render/RenderManager.h>

#include <glew.h>

namespace Wiwa {
	RenderLayer RenderManager::m_RenderLayers[MAX_LAYERS];
	FrameBuffer RenderManager::m_FrameBuffer;
	Shader RenderManager::m_Shader;
	uint32_t RenderManager::m_VAO;
	uint32_t RenderManager::m_VBO;
	uint32_t RenderManager::m_EBO;
	glm::mat4 RenderManager::m_OrthoProj;
	glm::mat4 RenderManager::m_View;
	glm::mat4 RenderManager::m_Model;
	uint32_t RenderManager::m_OrthoLoc;
	uint32_t RenderManager::m_ViewLoc;
	uint32_t RenderManager::m_ModelLoc;

	void RenderManager::Init(int width, int height)
	{
		// Quad vertices
		float vertices[] = {
			 // Positions			// Texture coords
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // Top right -> 1.0f, 1.0f
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // Bottom right -> 1.0f, 0.0f
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // Bottom left -> 0.0f, 0.0f
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f  // Top left -> 0.0f, 1.0f
		};

		// Quad indices
		unsigned int indexes[] = {
			0, 1, 3, 1, 2, 3
		};

		// Generate buffers
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		// Bind vertices
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Bind indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

		// Texture coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		// Render objects
		m_FrameBuffer.Init(width, height);
		m_Shader.Init("resources/shaders/renderlayer/renderlayer");

		m_OrthoLoc = m_Shader.getUniformLocation("u_Proj");
		m_ViewLoc = m_Shader.getUniformLocation("u_View");
		m_ModelLoc = m_Shader.getUniformLocation("u_Model");

		// Init orthographic projection
		m_OrthoProj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);
		// Init main camera view
		m_View = glm::mat4(1.0f);
		m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, -3.0f));
		// Init model
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, glm::vec3(width / 2.0f, height / 2.0f, 0.0f));
		m_Model = glm::scale(m_Model, glm::vec3((float)width, (float)height, 1.0f));
	}

	void RenderManager::Update()
	{
		// Set viewport
		glViewport(0, 0, m_FrameBuffer.getWidth(), m_FrameBuffer.getHeight());

		// Bind framebuffer
		m_FrameBuffer.Bind(false); // No clear so we clear without transparency
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind shader
		m_Shader.Bind();

		// Set MVP uniforms
		m_Shader.setUniform(m_OrthoLoc, m_OrthoProj);
		m_Shader.setUniform(m_ViewLoc, m_View);
		m_Shader.setUniform(m_ModelLoc, m_Model);

		// Bind VAO
		glBindVertexArray(m_VAO);

		for (int i = 0; i < MAX_LAYERS; i++) {
			// Take cameras in reverse order for proper drawing order
			Camera* cam = m_RenderLayers[MAX_LAYERS - i - 1].getCamera();
			if (cam) {
				// Bind texture
				glBindTexture(GL_TEXTURE_2D, cam->frameBuffer->getColorBufferTexture());
				// Draw elements
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}

		// Unbind shader and framebuffer
		m_Shader.UnBind();
		m_FrameBuffer.Unbind();
	}

	void RenderManager::Destroy()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}
}