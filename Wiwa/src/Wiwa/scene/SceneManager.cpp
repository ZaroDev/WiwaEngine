#include <wipch.h>
#include "SceneManager.h"

namespace Wiwa {
	SceneManager::SceneManager()
	{
		m_ActiveScene = -1;
		m_SceneSize = 0;
	}

	SceneManager::~SceneManager()
	{
		for (size_t i = 0; i < m_SceneSize; i++) {
			delete m_Scenes[i];
		}
	}

	void SceneManager::Update()
	{
		m_Scenes[m_ActiveScene]->Update();
	}

	size_t SceneManager::CreateScene()
	{
		size_t scene_id = m_Scenes.size();

		Scene* sc = new Scene();

		m_Scenes.push_back(sc);

		sc->Init();

		return scene_id;
	}

	void SceneManager::ChangeScene(size_t sceneId)
	{
		m_Scenes[m_ActiveScene]->Unload();
		m_ActiveScene = sceneId;
		m_Scenes[m_ActiveScene]->Load();
	}

	void SceneManager::StartChangeScene(size_t sceneId)
	{
		m_Scenes[m_ActiveScene]->ChangeScene(sceneId);
	}
}