#include <wipch.h>
#include "Scene.h"
#include "SceneManager.h"

#include <Wiwa/Application.h>

namespace Wiwa {
	Scene::Scene()
	{
		mMaxTimeEntering = 0;
		mMaxTimeLeaving = 0;
	}

	Scene::~Scene()
	{
	}

	void Scene::Update()
	{
		switch (m_CurrentState)
		{
		case Scene::SCENE_ENTERING:
			m_TransitionTimer += (size_t)Application::Get().getDeltaTime();
			if (m_TransitionTimer >= mMaxTimeEntering) m_CurrentState = SCENE_LOOP;
			UpdateEnter();
			RenderEnter();
			break;
		case Scene::SCENE_LOOP:
			ProcessInput();
			UpdateLoop();
			RenderLoop();
			break;
		case Scene::SCENE_LEAVING:
			m_TransitionTimer += (size_t)Application::Get().getDeltaTime();
			if (m_TransitionTimer >= mMaxTimeLeaving) Application::Get().GetSceneManager().ChangeScene(m_SceneToChange);
			UpdateLeave();
			RenderLeave();
			break;
		default:
			break;
		}
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