#pragma once

#include "Scene.h"

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/core/Layer.h>

#include <vector>

namespace Wiwa {
	class SceneManager : public Layer {
	private:
		std::vector<Scene*> m_Scenes;
		size_t m_SceneSize;
		size_t m_ActiveScene;
	public:
		SceneManager();
		~SceneManager();

		void Update();

		void ChangeScene(size_t sceneId);

		void StartChangeScene(size_t sceneId);

		size_t getActiveScene() { return m_ActiveScene; }

		inline size_t getSceneSize() { return m_SceneSize; }

		size_t CreateScene();
	};
}