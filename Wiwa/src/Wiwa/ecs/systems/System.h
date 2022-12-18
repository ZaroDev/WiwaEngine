#pragma once

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>

namespace Wiwa {
	class WI_API System {
	private:
		std::vector<EntityId> m_RegisteredEntities;

		size_t getEntityPos(EntityId eid);
	protected:
		template<class T> T* GetComponent(EntityId eid);

		virtual void OnAwake(EntityId eid){}

		virtual void OnInit(EntityId eid) {}

		virtual void OnUpdate(EntityId eid) {}
	public:
		System();
		virtual ~System(); // Virtual destructor, so that child destructor is called

		void Reserve(size_t amount);

		void AddEntity(EntityId entity);
		void RemoveEntity(EntityId entity);

		void Awake();
		void Init();
		void Update();
	};

	template<class T>
	inline T* System::GetComponent(EntityId eid)
	{
		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		T* component = em.GetComponent<T>(eid);

		return component;
	}
}