#pragma once
#include <vector>

namespace Wiwa {
	class Scene {
	public:
		Scene();
		~Scene();

		enum State {
			SCENE_ENTERING,
			SCENE_LOOP,
			SCENE_LEAVING
		};

		void Update();

		virtual void Init() {};
		virtual void Load();
		virtual void Unload() {}

		State getState() { return m_CurrentState; }

		void ChangeScene(size_t scene);
	protected:
		virtual void ProcessInput() {}

		virtual void UpdateEnter() {}
		virtual void UpdateLoop() {}
		virtual void UpdateLeave() {}

		virtual void RenderEnter() {}
		virtual void RenderLoop() {}
		virtual void RenderLeave() {}

		size_t mMaxTimeEntering, mMaxTimeLeaving = 0;
	private:
		State m_CurrentState = SCENE_ENTERING;
		size_t m_TransitionTimer = 0;
		size_t m_SceneToChange = 0;
	};
}