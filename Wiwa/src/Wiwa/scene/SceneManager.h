#pragma once

#include <Wiwa/core/Core.h>

#include "Scene.h"

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/core/Layer.h>

#include <vector>

typedef size_t SceneId;

namespace Wiwa {
	class WI_API SceneManager : public Layer {
	private:
		static std::vector<Scene*> m_Scenes;
		static SceneId m_ActiveScene;

		SceneManager();
	public:
		static void Update();
		static void CleanUp();
		
		static void SetScene(SceneId sceneId) { m_ActiveScene = sceneId; }
		static void ChangeScene(SceneId sceneId);

		static void StartChangeScene(SceneId sceneId);

		static SceneId getActiveSceneId() { return m_ActiveScene; }
		static Scene* getActiveScene() { return m_Scenes[m_ActiveScene]; }

		inline static size_t getSceneSize() { return m_Scenes.size(); }

		static size_t CreateScene();
		static Scene* getScene(SceneId sceneId) { return m_Scenes[sceneId]; }
	};
}