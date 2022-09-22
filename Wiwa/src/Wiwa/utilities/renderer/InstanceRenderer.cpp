#include "..\..\..\headers\utilities\renderer\InstanceRenderer.h"
#include "../../../Application.h"

EM::InstanceRenderer::InstanceRenderer(uint32 maxInstances) : m_MaxInstances(maxInstances)
{
}

EM::InstanceRenderer::~InstanceRenderer()
{

}

void EM::InstanceRenderer::Init()
{
	Application* app = Application::GetInstance();

	// Load shader
	m_InstanceShaderId = app->resources->Load<Shader>("Assets/Resources/Shaders/texcolorShader");
	m_InstanceShader = app->resources->GetResourceById<Shader>(m_InstanceShaderId);

	m_InstanceShader->Use();
	uint32 texs_id = glGetUniformLocation(m_InstanceShader->getID(), "u_Textures");

	// Set samplers id
	int samplers[16];
	for (int i = 0; i < 16; i++) samplers[i] = i;
	glUniform1iv(texs_id, 16, samplers);

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

	m_OrthoLocation = m_InstanceShader->getUniformLocation("u_Proj");
	m_ViewLocation = m_InstanceShader->getUniformLocation("u_View");
}

void EM::InstanceRenderer::Update()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_IVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexInstanceTexture) * m_InstanceCount, m_InstanceVertex);
}

void EM::InstanceRenderer::Render()
{
	m_InstanceShader->Use();
	m_InstanceShader->setUniform(m_OrthoLocation, Application::GetInstance()->renderer2D->GetOrthoProjection());
	m_InstanceShader->setUniform(m_ViewLocation, Application::GetInstance()->renderer2D->GetView());

	GLuint texSize = static_cast<GLuint>(m_Textures.size());

	glBindVertexArray(m_VAO);

	for (GLuint i = 0; i < texSize; i++) {
		glBindTextureUnit(i, m_Textures[i]);
	}

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_InstanceCount);
}

uint32 EM::InstanceRenderer::AddInstance(uint32 textureId, Vector2i& position, Size2i& size, Color4f& color, TextureClip& clip, Renderer2D::Pivot pivot)
{
	if (m_InstanceCount == m_MaxInstances)
		return -1;

	m_InstanceVertex[m_InstanceCount].position = Renderer2D::CalculateScreenGlPos(position, size, pivot);
	m_InstanceVertex[m_InstanceCount].scale = { static_cast<float>(size.x), static_cast<float>(size.y) };
	m_InstanceVertex[m_InstanceCount].color = color;

	Image* spr = Application::GetInstance()->resources->GetResourceById<Image>(textureId);

	int texid = AddTexture(spr->GetTextureId());

	m_InstanceVertex[m_InstanceCount].textureId = static_cast<float>(texid);

	m_InstanceVertex[m_InstanceCount].textureClip = clip;

	return m_InstanceCount++;
}

void EM::InstanceRenderer::UpdateInstance(uint32 id, Vector2i& position, Size2i& size, Color4f& color, Renderer2D::Pivot pivot)
{
	m_InstanceVertex[id].position = Renderer2D::CalculateScreenGlPos(position, size, pivot);
	m_InstanceVertex[id].scale = { static_cast<float>(size.x), static_cast<float>(size.y) };
	m_InstanceVertex[id].color = color;
}

void EM::InstanceRenderer::UpdateInstance(uint32 id, Vector2i& position, Renderer2D::Pivot pivot)
{
	Vector2i s = { (int)m_InstanceVertex[id].scale.w, (int)m_InstanceVertex->scale.h };
	m_InstanceVertex[id].position = Renderer2D::CalculateScreenGlPos(position, s, pivot);
}

int EM::InstanceRenderer::AddTexture(uint32 texture)
{
	size_t texSize = m_Textures.size();

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
