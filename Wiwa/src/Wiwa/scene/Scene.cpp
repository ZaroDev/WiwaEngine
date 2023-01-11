#include <wipch.h>
#include "Scene.h"
#include "SceneManager.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>

namespace Wiwa {
	Scene::Scene()
	{
		mMaxTimeEntering = 0;
		mMaxTimeLeaving = 0;
		m_CameraManager = new CameraManager();
	}

	Scene::~Scene()
	{
		
	}

	void Scene::Update()
	{
		m_CameraManager->Update();
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
		m_EntityManager.RegisterSystem<MeshRenderer>();
	}

	void Scene::Init()
	{
		m_EntityManager.RegisterSystem<MeshRenderer>();
	}

	void Scene::Load()
	{
		m_TransitionTimer = 0;

		m_CurrentState = SCENE_ENTERING;
	}

	void Scene::Unload()
	{
		Wiwa::Resources::UnloadSceneResources();
	}

	void Scene::ChangeScene(size_t scene)
	{
		m_SceneToChange = scene;
		m_CurrentState = SCENE_LEAVING;
		m_TransitionTimer = 0;
	}
}