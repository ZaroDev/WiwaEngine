#include <wipch.h>
#include "Renderer3D.h"

#include <Wiwa/core/Application.h>

#include <Wiwa/utilities/render/InstanceRenderer.h>
#include <Wiwa/utilities/Log.h>
#include <Wiwa/utilities/render/CameraManager.h>

#include <glew.h>

namespace Wiwa {
	Renderer3D::Renderer3D() {
	}

	Renderer3D::~Renderer3D()
	{
	}

	bool Renderer3D::Init()
	{
		Size2i& resolution = Application::Get().GetTargetResolution();

		// Init perspective camera for 3D
		m_ActiveCamera = new Camera();
		m_ActiveCamera->SetPerspective(45.0f, resolution.w / (float)resolution.h);
		m_ActiveCamera->setPosition({ 0.0f, 1.0f, 5.0f });
		m_ActiveCamera->lookat({ 0.0f, 0.0f, 0.0f });

		// Color shader
		m_ColorShaderId = Resources::Load<Shader>("resources/shaders/lit_model_color");
		m_ColorShader = Resources::GetResourceById<Shader>(m_ColorShaderId);

		m_CSColorUniformLocation = m_ColorShader->getUniformLocation("u_Color");
		m_CSDLUniforms.Direction = m_ColorShader->getUniformLocation("u_DirLight.direction");
		m_CSDLUniforms.Ambient = m_ColorShader->getUniformLocation("u_DirLight.ambient");
		m_CSDLUniforms.Diffuse = m_ColorShader->getUniformLocation("u_DirLight.diffuse");
		m_CSDLUniforms.Specular = m_ColorShader->getUniformLocation("u_DirLight.specular");
		
		m_CSUniforms.Diffuse = m_ColorShader->getUniformLocation("u_Material.diffuse");
		m_CSUniforms.Specular = m_ColorShader->getUniformLocation("u_Material.specular");
		m_CSUniforms.Shininess = m_ColorShader->getUniformLocation("u_Material.shininess");

		m_CSUniforms.ViewPos = m_ColorShader->getUniformLocation("u_ViewPos");

		m_CSUniforms.Model = m_ColorShader->getUniformLocation("u_Model");
		m_CSUniforms.View = m_ColorShader->getUniformLocation("u_View");
		m_CSUniforms.Projection = m_ColorShader->getUniformLocation("u_Proj");

		//Texture shader
		m_TextureShaderId = Resources::Load<Shader>("resources/shaders/lit_model_texture");
		m_TextureShader = Resources::GetResourceById<Shader>(m_TextureShaderId);

		m_TSDLUniforms.Direction = m_TextureShader->getUniformLocation("u_DirLight.direction");
		m_TSDLUniforms.Ambient = m_TextureShader->getUniformLocation("u_DirLight.ambient");
		m_TSDLUniforms.Diffuse = m_TextureShader->getUniformLocation("u_DirLight.diffuse");
		m_TSDLUniforms.Specular = m_TextureShader->getUniformLocation("u_DirLight.specular");

		m_TSPLUniforms.LightsNum = m_TextureShader->getUniformLocation("u_PointLightNum");

		m_TSUniforms.Diffuse = m_TextureShader->getUniformLocation("u_Material.diffuse");
		m_TSUniforms.Specular = m_TextureShader->getUniformLocation("u_Material.specular");
		m_TSUniforms.Shininess = m_TextureShader->getUniformLocation("u_Material.shininess");

		m_TSUniforms.ViewPos = m_TextureShader->getUniformLocation("u_ViewPos");


		m_TSUniforms.Model = m_TextureShader->getUniformLocation("u_Model");
		m_TSUniforms.View = m_TextureShader->getUniformLocation("u_View");
		m_TSUniforms.Projection = m_TextureShader->getUniformLocation("u_Proj");

		//Normal Display Shader
		m_NormalDisplayShaderId = Resources::Load<Shader>("resources/shaders/normal_display");
		m_NormalDisplayShader = Resources::GetResourceById<Shader>(m_NormalDisplayShaderId);

		m_NDSUniforms.Model = m_NormalDisplayShader->getUniformLocation("u_Model");
		m_NDSUniforms.View = m_NormalDisplayShader->getUniformLocation("u_View");
		m_NDSUniforms.Projection = m_NormalDisplayShader->getUniformLocation("u_Projection");
		
		//Bounding box display shader
		m_BBDisplayShaderId = Resources::Load<Shader>("resources/shaders/boundingbox_display");
		m_BBDisplayShader = Resources::GetResourceById<Shader>(m_BBDisplayShaderId);

		m_BBDSUniforms.Model = m_BBDisplayShader->getUniformLocation("u_Model");
		m_BBDSUniforms.View = m_BBDisplayShader->getUniformLocation("u_View");
		m_BBDSUniforms.Projection = m_BBDisplayShader->getUniformLocation("u_Projection");

		WI_CORE_INFO("Renderer3D initialized");
		SetOption(Options::DEPTH_TEST);
		SetOption(Options::CULL_FACE);
		return true;
	}

	void Renderer3D::Update()
	{
		m_ActiveCamera->frameBuffer->Clear();
	}

	void Renderer3D::RenderMeshColor(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, bool clear, Camera* camera, bool cull)
	{
		if (!camera) camera = m_ActiveCamera;

		FrameBuffer* target = camera->frameBuffer;

		glViewport(0, 0, target->getWidth(), target->getHeight());

		target->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
		model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

		glm::vec3 directionLight = { target->getDirectionalLight().Direction.x, target->getDirectionalLight().Direction.y , target->getDirectionalLight().Direction.z };
		glm::vec3 ambientLight = { target->getDirectionalLight().Ambient.r, target->getDirectionalLight().Ambient.g , target->getDirectionalLight().Ambient.b };
		glm::vec3 diffuseLight = { target->getDirectionalLight().Diffuse.r, target->getDirectionalLight().Diffuse.g , target->getDirectionalLight().Diffuse.b };
		glm::vec3 specularLight = { target->getDirectionalLight().Specular.r, target->getDirectionalLight().Specular.g , target->getDirectionalLight().Specular.b };
		
		m_ColorShader->Bind();

		m_ColorShader->setUniform(m_CSDLUniforms.Direction, directionLight);
		m_ColorShader->setUniform(m_CSDLUniforms.Ambient, ambientLight);
		m_ColorShader->setUniform(m_CSDLUniforms.Diffuse, diffuseLight);
		m_ColorShader->setUniform(m_CSDLUniforms.Specular, specularLight);

		m_ColorShader->setUniform(m_CSUniforms.Diffuse, material->getSettings().diffuse);
		m_ColorShader->setUniform(m_CSUniforms.Specular, material->getSettings().specular);
		m_ColorShader->setUniform(m_CSUniforms.Shininess, material->getSettings().shininess);

		m_ColorShader->setUniform(m_CSUniforms.ViewPos, camera->getPosition());

		glm::vec4 color = { material->getColor().r, material->getColor().g, material->getColor().b, material->getColor().a };

		m_ColorShader->setUniform(m_CSColorUniformLocation, color);
		
		m_ColorShader->setUniform(m_CSUniforms.Model, model);
		m_ColorShader->setUniform(m_CSUniforms.View, camera->getView());
		m_ColorShader->setUniform(m_CSUniforms.Projection, camera->getProjection());

		mesh->Render();

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, model);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		target->Unbind();
		m_ColorShader->UnBind();
	}

	void Renderer3D::RenderMeshMaterial(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, bool clear, Camera* camera, bool cull)
	{

		if (!camera)
		{
			camera = m_ActiveCamera;
		}
		

		if (material->getType() == Wiwa::Material::MaterialType::color)
		{
			RenderMeshColor(mesh, position, rotation, scale, material, clear, camera, cull);
			return;
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
		model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

		m_TextureShader->Bind();


		glm::vec3 postitionLight = { camera->frameBuffer->getDirectionalLight().Direction.x, camera->frameBuffer->getDirectionalLight().Direction.y , camera->frameBuffer->getDirectionalLight().Direction.z };
		glm::vec3 ambientLight = { camera->frameBuffer->getDirectionalLight().Ambient.r, camera->frameBuffer->getDirectionalLight().Ambient.g , camera->frameBuffer->getDirectionalLight().Ambient.b };
		glm::vec3 diffuseLight = { camera->frameBuffer->getDirectionalLight().Diffuse.r, camera->frameBuffer->getDirectionalLight().Diffuse.g , camera->frameBuffer->getDirectionalLight().Diffuse.b };
		glm::vec3 specularLight = { camera->frameBuffer->getDirectionalLight().Specular.r, camera->frameBuffer->getDirectionalLight().Specular.g , camera->frameBuffer->getDirectionalLight().Specular.b };

		m_TextureShader->setUniform(m_TSPLUniforms.Position, (unsigned int)camera->frameBuffer->getPointLights()->size());

		/*for (int i = 0; i < target->getPointLights()->size(); i++)
		{
			std::string num = std::to_string(i);
			m_TextureShader->setUniform(glGetUniformLocation(m_TextureShader->getID(), ("u_PointLight[" + num + "].position").c_str()), target->getPointLights()->at(i).Position);
			m_TextureShader->setUniform(glGetUniformLocation(m_TextureShader->getID(), ("u_PointLight[" + num + "].constant").c_str()), target->getPointLights()->at(i).Constant);
			m_TextureShader->setUniform(glGetUniformLocation(m_TextureShader->getID(), ("u_PointLight[" + num + "].linear").c_str()), target->getPointLights()->at(i).Linear);
			m_TextureShader->setUniform(glGetUniformLocation(m_TextureShader->getID(), ("u_PointLight[" + num + "].quadratic").c_str()), target->getPointLights()->at(i).Quadratic);
			m_TextureShader->setUniform(glGetUniformLocation(m_TextureShader->getID(), ("u_PointLight[" + num + "].ambient").c_str()), target->getPointLights()->at(i).Ambient);
			m_TextureShader->setUniform(glGetUniformLocation(m_TextureShader->getID(), ("u_PointLight[" + num + "].diffuse").c_str()), target->getPointLights()->at(i).Diffuse);
			m_TextureShader->setUniform(glGetUniformLocation(m_TextureShader->getID(), ("u_PointLight[" + num + "].specular").c_str()), target->getPointLights()->at(i).Specular);
		}*/

		m_TextureShader->setUniform(m_TSDLUniforms.Direction, postitionLight);
		m_TextureShader->setUniform(m_TSDLUniforms.Ambient, ambientLight);
		m_TextureShader->setUniform(m_TSDLUniforms.Diffuse, diffuseLight);
		m_TextureShader->setUniform(m_TSDLUniforms.Specular, specularLight);

		m_TextureShader->setUniform(m_TSUniforms.Diffuse, mesh->getVAO());
		m_TextureShader->setUniform(m_TSUniforms.Specular, mesh->getVAO());
		m_TextureShader->setUniform(m_TSUniforms.Shininess, material->getSettings().shininess);

		m_TextureShader->setUniform(m_TSUniforms.ViewPos, camera->getPosition());

		m_TextureShader->setUniform(m_TSUniforms.Model, model);
		m_TextureShader->setUniform(m_TSUniforms.View, camera->getView());
		m_TextureShader->setUniform(m_TSUniforms.Projection, camera->getProjection());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->getTextureId());

		mesh->Render();
		glBindTexture(GL_TEXTURE_2D, 0);
		m_TextureShader->UnBind();
		if (mesh->showNormals)
		{
			glBindTexture(GL_TEXTURE_2D, 0);

			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, model);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();

			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, model);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			m_BBDisplayShader->setUniform(glGetUniformLocation(m_BBDisplayShader->getID(), "u_Min"), mesh->boundingBox.getMin());
			m_BBDisplayShader->setUniform(glGetUniformLocation(m_BBDisplayShader->getID(), "u_Max"), mesh->boundingBox.getMax());
			mesh->Render();
			m_BBDisplayShader->UnBind();
		}
		
		camera->frameBuffer->Unbind();
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
		delete m_ActiveCamera;
	}
}