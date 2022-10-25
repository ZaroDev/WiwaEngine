#include <wipch.h>
#include "Renderer3D.h"

#include <Wiwa/Application.h>

#include <Wiwa/utilities/render/InstanceRenderer.h>
#include <Wiwa/Log.h>

#include <glew.h>

namespace Wiwa {
	Renderer3D::Renderer3D() {
		Light light = { 
			glm::vec3{0.0f, 10.0f, 0.0f},	//Position
			glm::vec3{1.0f, 1.0f, 1.0f},		//Ambient
			glm::vec3{1.0f, 1.0f, 1.0f},		//Diffuse
			glm::vec3{1.0f, 1.0f, 1.0f}		//Specular
		};
		m_FrameBuffer.setLight(light);
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
		m_ActiveCamera.lookat({ 0.0f, 0.0f, 0.0f });

		// Color shader
		m_ColorShaderId = Resources::Load<Shader>("resources/shaders/lit_model_color");
		m_ColorShader = Resources::GetResourceById<Shader>(m_ColorShaderId);

		m_CSColorUniformLocation = m_ColorShader->getUniformLocation("u_Color");

		m_CSUniforms.MatAmbient = m_ColorShader->getUniformLocation("u_Material.ambient");
		m_CSUniforms.MatDiffuse = m_ColorShader->getUniformLocation("u_Material.diffuse");
		m_CSUniforms.MatSpecular = m_ColorShader->getUniformLocation("u_Material.specular");
		m_CSUniforms.Shininess = m_ColorShader->getUniformLocation("u_Material.shininess");

		m_CSUniforms.ViewPos = m_ColorShader->getUniformLocation("u_ViewPos");

		m_CSUniforms.Model = m_ColorShader->getUniformLocation("u_Model");
		m_CSUniforms.View = m_ColorShader->getUniformLocation("u_View");
		m_CSUniforms.Projection = m_ColorShader->getUniformLocation("u_Proj");

		m_TextureShaderId = Resources::Load<Shader>("resources/shaders/lit_model_texture");
		m_TextureShader = Resources::GetResourceById<Shader>(m_TextureShaderId);

		m_TSUniforms.LigPos = m_TextureShader->getUniformLocation("u_Light.position");
		m_TSUniforms.LigAmbient = m_TextureShader->getUniformLocation("u_Light.ambient");
		m_TSUniforms.LigDiffuse = m_TextureShader->getUniformLocation("u_Light.diffuse");
		m_TSUniforms.LigSpecular = m_TextureShader->getUniformLocation("u_Light.specular");

		m_TSUniforms.MatAmbient = m_TextureShader->getUniformLocation("u_Material.ambient");
		m_TSUniforms.MatDiffuse = m_TextureShader->getUniformLocation("u_Material.diffuse");
		m_TSUniforms.MatSpecular = m_TextureShader->getUniformLocation("u_Material.specular");
		m_TSUniforms.Shininess = m_TextureShader->getUniformLocation("u_Material.shininess");

		m_TSUniforms.ViewPos = m_TextureShader->getUniformLocation("u_ViewPos");


		m_TSUniforms.Model = m_TextureShader->getUniformLocation("u_Model");
		m_TSUniforms.View = m_TextureShader->getUniformLocation("u_View");
		m_TSUniforms.Projection = m_TextureShader->getUniformLocation("u_Proj");

		
		WI_CORE_INFO("Renderer3D initialized");
		SetOption(Options::DEPTH_TEST);
		SetOption(Options::CULL_FACE);
		return true;
	}

	void Renderer3D::Update()
	{
		m_FrameBuffer.Clear();
	}

	void Renderer3D::RenderMeshColor(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, bool clear, FrameBuffer* target, Camera* camera)
	{
		if (!target) target = &m_FrameBuffer;
		if (!camera) camera = &m_ActiveCamera;

		glViewport(0, 0, target->getWidth(), target->getHeight());

		target->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
		model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

		glm::vec3 postitionLight = { target->getLight().Position.x, target->getLight().Position.y , target->getLight().Position.z };
		glm::vec3 ambientLight = { target->getLight().Ambient.r, target->getLight().Ambient.g , target->getLight().Ambient.b };
		glm::vec3 diffuseLight = { target->getLight().Diffuse.r, target->getLight().Diffuse.g , target->getLight().Diffuse.b };
		glm::vec3 specularLight = { target->getLight().Specular.r, target->getLight().Specular.g , target->getLight().Specular.b };
		
		m_ColorShader->Use();

		m_ColorShader->setUniform(m_CSUniforms.LigPos, postitionLight);
		m_ColorShader->setUniform(m_CSUniforms.LigAmbient, ambientLight);
		m_ColorShader->setUniform(m_CSUniforms.LigDiffuse, diffuseLight);
		m_ColorShader->setUniform(m_CSUniforms.LigSpecular, specularLight);

		m_ColorShader->setUniform(m_TSUniforms.MatAmbient, material->getSettings().ambient);
		m_ColorShader->setUniform(m_TSUniforms.MatDiffuse, material->getSettings().diffuse);
		m_ColorShader->setUniform(m_TSUniforms.MatSpecular, material->getSettings().specular);
		m_ColorShader->setUniform(m_TSUniforms.Shininess, material->getSettings().shininess);

		m_ColorShader->setUniform(m_CSUniforms.ViewPos, camera->getPosition());

		glm::vec4 color = { material->getColor().r, material->getColor().g, material->getColor().b, material->getColor().a };

		m_ColorShader->setUniform(m_CSColorUniformLocation, color);
		
		m_ColorShader->setUniform(m_CSUniforms.Model, model);
		m_ColorShader->setUniform(m_CSUniforms.View, camera->getView());
		m_ColorShader->setUniform(m_CSUniforms.Projection, camera->getProjection());

		mesh->Render();

		target->Unbind();
	}

	void Renderer3D::RenderMeshMaterial(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, bool clear, FrameBuffer* target, Camera* camera)
	{
		if (!target) target = &m_FrameBuffer;
		if (!camera) camera = &m_ActiveCamera;

		if (material->getType() == Wiwa::Material::MaterialType::color)
		{
			RenderMeshColor(mesh, position, rotation, scale, material, clear, target, camera);
			return;
		}

		glViewport(0, 0, target->getWidth(), target->getHeight());

		target->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
		model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

		m_TextureShader->Use();
		glBindVertexArray(mesh->getVAO());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->getTextureId());

		glm::vec3 postitionLight = { target->getLight().Position.x, target->getLight().Position.y , target->getLight().Position.z };
		glm::vec3 ambientLight = { target->getLight().Ambient.r, target->getLight().Ambient.g , target->getLight().Ambient.b };
		glm::vec3 diffuseLight = { target->getLight().Diffuse.r, target->getLight().Diffuse.g , target->getLight().Diffuse.b };
		glm::vec3 specularLight = { target->getLight().Specular.r, target->getLight().Specular.g , target->getLight().Specular.b };



		m_TextureShader->setUniform(m_TSUniforms.LigPos, postitionLight);
		m_TextureShader->setUniform(m_TSUniforms.LigAmbient, ambientLight);
		m_TextureShader->setUniform(m_TSUniforms.LigDiffuse, diffuseLight);
		m_TextureShader->setUniform(m_TSUniforms.LigSpecular, specularLight);

		m_TextureShader->setUniform(m_TSUniforms.MatAmbient, material->getSettings().ambient);
		m_TextureShader->setUniform(m_TSUniforms.MatDiffuse, material->getSettings().diffuse);
		m_TextureShader->setUniform(m_TSUniforms.MatSpecular, material->getSettings().specular);
		m_TextureShader->setUniform(m_TSUniforms.Shininess, material->getSettings().shininess);

		m_TextureShader->setUniform(m_TSUniforms.ViewPos, camera->getPosition());

		m_TextureShader->setUniform(m_TSUniforms.Model, model);
		m_TextureShader->setUniform(m_TSUniforms.View, camera->getView());
		m_TextureShader->setUniform(m_TSUniforms.Projection, camera->getProjection());


		mesh->Render();

		glBindTexture(GL_TEXTURE_2D, 0);
		target->Unbind();
	}

	void Renderer3D::RenderGrid(Model* grid, FrameBuffer* target, bool clear, Camera* camera)
	{
		if (!target) target = &m_FrameBuffer;
		if (!camera) camera = &m_ActiveCamera;
		/*glViewport(0, 0, target->getWidth(), target->getHeight());

		target->Bind(clear);
		glBindVertexArray(grid->getVAO());

		glDrawElements(GL_LINES, grid., GL_UNSIGNED_INT, NULL);

		glBindVertexArray(0);
		target->Unbind();*/
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



	}

	void Renderer3D::Close()
	{

	}
}