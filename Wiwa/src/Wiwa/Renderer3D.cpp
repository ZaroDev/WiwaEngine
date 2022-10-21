#include <wipch.h>
#include "Renderer3D.h"

#include <Wiwa/Application.h>

#include <Wiwa/utilities/render/InstanceRenderer.h>
#include <Wiwa/Log.h>

#include <glew.h>

namespace Wiwa {
	Renderer3D::Renderer3D() {
		
	}

	Renderer3D::~Renderer3D()
	{
	}

	bool Renderer3D::Init()
	{
		Window& window = Application::Get().GetWindow();
		Size2i& resolution = Application::Get().GetTargetResolution();

		// Init framebuffer
		m_FrameBuffer.Init(resolution.w, resolution.h);

		// Init perspective camera for 3D
		m_ActiveCamera.SetPerspective(45.0f, resolution.w / (float)resolution.h);
		m_ActiveCamera.setPosition({ 0.0f, 1.0f, 5.0f });
		m_ActiveCamera.lookat({0.0f, 0.0f, 0.0f});

		// Color shader
		m_ColorShaderId = Resources::Load<Shader>("resources/shaders/model_color");
		m_ColorShader = Resources::GetResourceById<Shader>(m_ColorShaderId);

		m_CSColorUniformLocation = m_ColorShader->getUniformLocation("u_Color");

		m_CSModelUniformLocation = m_ColorShader->getUniformLocation("u_Model");
		m_CSViewUniformLocation = m_ColorShader->getUniformLocation("u_View");
		m_CSProjectionUniformLocation = m_ColorShader->getUniformLocation("u_Proj");

		m_TextureShaderId = Resources::Load<Shader>("resources/shaders/model_texture");
		m_TextureShader = Resources::GetResourceById<Shader>(m_TextureShaderId);

		/*m_TextureShader->Use();*/

		/*uint32_t texs_id = glGetUniformLocation(m_TextureShader->getID(), "u_Sampler");
		int samplers[MAX_INSTANCE_TEXTURES];
		for (int i = 0; i < MAX_INSTANCE_TEXTURES; i++) samplers[i] = i;
		glUniform1iv(texs_id, MAX_INSTANCE_TEXTURES, samplers);*/

		m_TSModelUniformLocation = m_TextureShader->getUniformLocation("u_Model");
		m_TSViewUniformLocation = m_TextureShader->getUniformLocation("u_View");
		m_TSProjectionUniformLocation = m_TextureShader->getUniformLocation("u_Proj");

		SetOption(Options::CULL_FACE);
		SetOption(Options::DEPTH_TEST);

		WI_CORE_INFO("Renderer3D initialized");
		return true;
	}

	void Renderer3D::Update()
	{
		
	}

	void Renderer3D::RenderMeshColor(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Color4f color, FrameBuffer* target, Camera* camera)
	{
		if (!target) target = &m_FrameBuffer;
		if (!camera) camera = &m_ActiveCamera;

		glViewport(0, 0, target->getWidth(), target->getHeight());

		target->Bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
		// TODO: ROTATION
		model = glm::rotate(model, rotation.x, { 1, 0, 0 });
		model = glm::rotate(model, rotation.y, { 0, 1, 0 });
		model = glm::rotate(model, rotation.z, { 0, 0, 1 });
		model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

		m_ColorShader->Use();

		m_ColorShader->setUniform(m_CSColorUniformLocation, glm::vec4(color.r, color.g, color.b, color.a));
		m_ColorShader->setUniform(m_CSModelUniformLocation, model);
		m_ColorShader->setUniform(m_CSViewUniformLocation, camera->getView());
		m_ColorShader->setUniform(m_CSProjectionUniformLocation, camera->getProjection());

		mesh->Render();

		target->Unbind();
	}

	void Renderer3D::RenderMeshMaterial(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, FrameBuffer* target, Camera* camera)
	{
		if (!target) target = &m_FrameBuffer;
		if (!camera) camera = &m_ActiveCamera;

		if (material->getType() == Wiwa::Material::MaterialType::color)
		{
			RenderMeshColor(mesh, position, rotation, scale, material->getColor(), target, camera);
			return;
		}

		glViewport(0, 0, target->getWidth(), target->getHeight());

		target->Bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
		// TODO: ROTATION
		model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

		m_TextureShader->Use();
		glBindVertexArray(mesh->getVAO());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->getTextureId());

		m_TextureShader->setUniform(m_TSModelUniformLocation, model);
		m_TextureShader->setUniform(m_TSViewUniformLocation, camera->getView());
		m_TextureShader->setUniform(m_TSProjectionUniformLocation, camera->getProjection());

		mesh->Render();

		glBindTexture(GL_TEXTURE_2D,0);
		target->Unbind();
	}


	void Renderer3D::SetOption(Options option)
	{
		switch (option)
		{
		case Wiwa::Renderer3D::DEPTH_TEST:
			glEnable(GL_DEPTH_TEST);
			break;
		case Wiwa::Renderer3D::CULL_FACE:
			glEnable(GL_CULL_FACE);
			break;
		case Wiwa::Renderer3D::LIGHTING:
			glEnable(GL_LIGHTING);
			break;
		case Wiwa::Renderer3D::COLOR_MATERIAL:
			glEnable(GL_COLOR_MATERIAL);
			break;
		case Wiwa::Renderer3D::TEXTURE_2D:
			glEnable(GL_TEXTURE_2D);
			break;
		case Wiwa::Renderer3D::WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		default:
			break;
		}
		WI_CORE_INFO("Option: {0} enabled!", option);
	}

	void Renderer3D::DisableOption(Options option)
	{
		switch (option)
		{
		case Wiwa::Renderer3D::DEPTH_TEST:
			glDisable(GL_DEPTH_TEST);
			break;
		case Wiwa::Renderer3D::CULL_FACE:
			glDisable(GL_CULL_FACE);
			break;
		case Wiwa::Renderer3D::LIGHTING:
			glDisable(GL_LIGHTING);
			break;
		case Wiwa::Renderer3D::COLOR_MATERIAL:
			glDisable(GL_COLOR_MATERIAL);
			break;
		case Wiwa::Renderer3D::TEXTURE_2D:
			glDisable(GL_TEXTURE_2D);
			break;
		case Wiwa::Renderer3D::WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
			break;
		}
		WI_CORE_INFO("Option: {0} disabled!", option);


	}

	void Renderer3D::Close()
	{
		
	}
}