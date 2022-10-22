#pragma once

#include <Wiwa/Core.h>
#include <Wiwa/Application.h>

typedef size_t EntityId;
typedef size_t ComponentId;
typedef size_t SystemId;

namespace Wiwa {
	template<class T, class T2, class... TArgs>
	class System {
	private:
		std::vector<size_t> m_RemovedIndex;
		std::vector<EntityId> m_RegisteredEntities;

		std::vector<void*> m_Components[sizeof...(TArgs) + 2];

		ComponentId m_ComponentIds[sizeof...(TArgs) + 2];
		size_t m_ComponentSize[sizeof...(TArgs) + 2];

		size_t getEntityPos(EntityId eid);

		void AddEntity(EntityId entity);
		void RemoveEntity(size_t index);

		size_t m_ComponentCount;
		size_t m_ComponentIdCount;

		template<class C> void GetComponentECSIds();
		template<class C, class C2, class ...CArgs> void GetComponentECSIds();

		void LoadComponents(size_t index);
		template<class C> void InsertComponents(size_t index);
		template<class C, class C2, class ...CArgs> void InsertComponents(size_t index);
		template<class C> size_t GetComponentIndex();
	protected:
		// Callbacks for sub-systems
		virtual void OnEntityAdded(EntityId entityID) {}
		virtual void OnEntityRemoved() {}

		template<class C> std::vector<C*>* GetComponents();
	public:
		System();
		virtual ~System(); // Virtual destructor, so that child destructor is called

		void Reserve(size_t amount);

		bool OnEntityComponentAdded(EntityId entity);
		void OnEntityRemoved(EntityId entity);

		void Init();
		void Update();

		virtual void OnInit(){}
		virtual void OnUpdate(){}
	};

	//-------- CONSTRUCTOR --------
	template<class T, class T2, class ...TArgs>
	inline System<T, T2, TArgs...>::System() : m_ComponentIdCount(0)
	{
		m_ComponentCount = sizeof...(TArgs) + 2;

		GetComponentECSIds<T, T2, TArgs...>();
	}

	//-------- DESTRUCTOR --------
	template<class T, class T2, class ...TArgs>
	inline System<T, T2, TArgs...>::~System()
	{
	}

	//-------- Entity Functions --------
	template<class T, class T2, class ...TArgs>
	inline size_t System<T, T2, TArgs...>::getEntityPos(EntityId eid)
	{
		size_t i;
		size_t size = m_RegisteredEntities.size();

		for (i = 0; i < size; i++) {
			if (m_RegisteredEntities[i] == eid) {
				break;
			}
		}

		return i;
	}

	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::AddEntity(EntityId entity)
	{
		size_t removedCount = m_RemovedIndex.size();

		size_t index = 0;

		if (removedCount > 0) {
			index = m_RemovedIndex[removedCount - 1];

			m_RemovedIndex.erase(m_RemovedIndex.begin() + removedCount - 1);

			m_RegisteredEntities[index] = entity;
		}
		else {
			index = m_RegisteredEntities.size();

			m_RegisteredEntities.emplace_back(entity);

			for (size_t i = 0; i < m_ComponentCount; i++) {
				m_Components[i].push_back(NULL);
			}
		}

		LoadComponents(index);
	
		// Callback for child system
		OnEntityAdded(entity);
	}

	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::RemoveEntity(size_t index)
	{
		for (size_t i = 0; i < m_ComponentCount; i++) {
			m_Components[i][index] = NULL;
		}

		m_RemovedIndex.push_back(index);

		// Callback for child system
		OnEntityRemoved();
	}

	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::OnEntityRemoved(EntityId entity)
	{
		size_t pos = getEntityPos(entity);

		// If entity is in the list
		if (pos < m_RegisteredEntities.size()) {
			RemoveEntity(pos);
		}
	}

	//-------- Component Functions -------- TODO: FIX COMPONENT LOAD TO TAKE POSITION INSTEAD OF COMPONENT POINTER
	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::LoadComponents(size_t index)
	{
		Application& app = Application::Get();

		for (size_t i = 0; i < m_ComponentCount; i++) {
			m_Components[i][index] = app.GetEntityManager().GetComponent(m_RegisteredEntities[index], m_ComponentIds[i], m_ComponentSize[i]);
		}
	}

	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::Reserve(size_t amount)
	{
		m_RegisteredEntities.reserve(amount);

		for (size_t i = 0; i < m_ComponentCount; i++) {
			m_Components[i].reserve(amount);
		}
	}

	template<class T, class T2, class ...TArgs>
	inline bool System<T, T2, TArgs...>::OnEntityComponentAdded(EntityId entity)
	{
		Application& app = Application::Get();

		bool valid = app.GetEntityManager().HasComponents(entity, m_ComponentIds, m_ComponentCount);

		if (valid) {
			AddEntity(entity);
		}

		return valid;
	}

	template<class T, class T2, class ...TArgs>
	template<class C>
	inline void System<T, T2, TArgs...>::GetComponentECSIds()
	{
		Application& app = Application::Get();

		size_t c_i = GetComponentIndex<C>();

		m_ComponentIds[c_i] = app.GetEntityManager().GetComponentId<C>();
		m_ComponentSize[c_i] = sizeof(C);
	}

	template<class T, class T2, class ...TArgs>
	template<class C, class C2, class ...CArgs>
	inline void System<T, T2, TArgs...>::GetComponentECSIds()
	{
		GetComponentECSIds<C>();
		GetComponentECSIds<C2, CArgs...>();
	}

	template<class T, class T2, class ...TArgs>
	template<class C>
	inline void System<T, T2, TArgs...>::InsertComponents(size_t index)
	{
		size_t cid = GetComponentIndex<C>();

		Application& app = Application::Get();

		m_Components[cid][index] = app.GetEntityManager().GetComponent(m_RegisteredEntities[index], m_ComponentIds[cid], m_ComponentSize[cid]);
	}

	template<class T, class T2, class ...TArgs>
	template<class C, class C2, class ...CArgs>
	inline void System<T, T2, TArgs...>::InsertComponents(size_t index)
	{
		InsertComponents<C>(index);
		InsertComponents<C2, CArgs...>(index);
	}

	template<class T, class T2, class ...TArgs>
	template<class C>
	inline size_t System<T, T2, TArgs...>::GetComponentIndex()
	{
		static size_t c_id = m_ComponentIdCount++;

		return c_id;
	}
	template<class T, class T2, class ...TArgs>
	template<class C>
	inline std::vector<C*>* System<T, T2, TArgs...>::GetComponents()
	{
		size_t c_id = GetComponentIndex<C>();

		std::vector<C*>* c_list = NULL;

		if (c_id < m_ComponentCount) {
			c_list = (std::vector<C*>*) & m_Components[c_id];
		}

		return c_list;
	}

	//-------- Sub-system functions --------
	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::Init()
	{
		OnInit();
	}

	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::Update()
	{
		OnUpdate();
	}
}