#include <wipch.h>
#include "SceneManager.h"

namespace Wiwa {
	std::vector<Scene*> SceneManager::m_Scenes;
	SceneId SceneManager::m_ActiveScene = -1;

	void SceneManager::Update()
	{
		m_Scenes[m_ActiveScene]->Update();
	}

	void SceneManager::CleanUp()
	{
		size_t scsize = m_Scenes.size();

		for (size_t i = 0; i < scsize; i++) {
			delete m_Scenes[i];
		}
	}

	SceneId SceneManager::CreateScene()
	{
		SceneId scene_id = m_Scenes.size();

		Scene* sc = new Scene();

		m_Scenes.push_back(sc);

		sc->Init();

		return scene_id;
	}

	void SceneManager::ChangeScene(SceneId sceneId)
	{
		m_Scenes[m_ActiveScene]->Unload();
		m_ActiveScene = sceneId;
		m_Scenes[m_ActiveScene]->Load();
	}

	void SceneManager::StartChangeScene(SceneId sceneId)
	{
		m_Scenes[m_ActiveScene]->ChangeScene(sceneId);
	}
}