#include <wipch.h>
#include "Renderer3D.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/utilities/Log.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <glew.h>
#include <Wiwa/utilities/render/shaders/Shader.h>

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

		WI_CORE_INFO("Renderer3D initialized");
		SetOption(Options::DEPTH_TEST);
		SetOption(Options::CULL_FACE);
		return true;
	}

	void Renderer3D::Update()
	{
		m_ActiveCamera->frameBuffer->Clear();
	}



	void Renderer3D::RenderMesh(Model* mesh, Vector3f position, Vector3f rotation, Vector3f scale, Material* material, bool clear/*=false*/, Camera* camera/*=NULL*/, bool cull /*= false*/)
	{
		if (!camera)
		{
			camera = m_ActiveCamera;
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

		material->getShader()->Bind();
		
		material->getShader()->SetMVP(model, camera->getView(), camera->getProjection());
		
		material->Bind();

		mesh->Render();

		material->UnBind();
		
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
	void Renderer3D::RenderFrustrums(Camera* camera)
	{
		if (!camera)
		{
			camera = m_ActiveCamera;
		}
		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		CameraManager& cameraManager = SceneManager::getActiveScene()->GetCameraManager();
		camera->frameBuffer->Bind(false);
		size_t cameraCount = cameraManager.getCameraSize();
		std::vector<CameraId>& cameras = cameraManager.getCameras();
		/*for (size_t i = 0; i < cameraCount; i++)
		{
			CameraId cam_id = cameras[i];
			Camera* cam = cameraManager.getCamera(cam_id);
			if (camera == cam)
				continue;

			m_BBDisplayShader->Bind();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cam->getPosition());
			model = glm::rotate(model, 0.0f, glm::vec3(1, 0, 0));
			model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));
			model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
			model = glm::scale(model, glm::vec3(1.0f));

			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, model);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			cam->DrawFrustrum();

			m_BBDisplayShader->UnBind();
		}*/
		camera->frameBuffer->Unbind();
	}
}