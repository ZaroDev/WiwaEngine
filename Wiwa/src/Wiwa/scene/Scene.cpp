#include <wipch.h>
#include "Scene.h"
#include "SceneManager.h"

#include <Wiwa/core/Application.h>

namespace Wiwa {
	Scene::Scene()
	{
		mMaxTimeEntering = 0;
		mMaxTimeLeaving = 0;
		Size2i& resolution = Application::Get().GetTargetResolution();
		m_SceneCameraId = Wiwa::CameraManager::CreatePerspectiveCamera(45.0f, (float)resolution.w / (float)resolution.h);
		m_SceneCamera = Wiwa::CameraManager::getCamera(m_SceneCameraId);
		m_SceneCamera->setPosition({ 10.0f, 1.0f, 5.0f });
		m_SceneCamera->lookat({ 0.0f, 0.0f, 0.0f });
	}

	Scene::~Scene()
	{
		delete m_SceneCamera;
	}

	void Scene::Update()
	{
		switch (m_CurrentState)
		{
		case Scene::SCENE_ENTERING:
			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			if (m_TransitionTimer >= mMaxTimeEntering) m_CurrentState = SCENE_LOOP;
			UpdateEnter();
			RenderEnter();
			break;
		case Scene::SCENE_LOOP:
			m_EntityManager.Update();

			ProcessInput();
			UpdateLoop();
			RenderLoop();
			break;
		case Scene::SCENE_LEAVING:
			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			if (m_TransitionTimer >= mMaxTimeLeaving) SceneManager::ChangeScene(m_SceneToChange);
			UpdateLeave();
			RenderLeave();
			break;
		default:
			break;
		}
	}

	void Scene::Start()
	{
		Wiwa::CameraManager::setActiveCamera(m_SceneCameraId);
	}

	void Scene::Load()
	{
		m_TransitionTimer = 0;

		m_CurrentState = SCENE_ENTERING;
	}

	void Scene::ChangeScene(size_t scene)
	{
		m_SceneToChange = scene;
		m_CurrentState = SCENE_LEAVING;
		m_TransitionTimer = 0;
	}
}