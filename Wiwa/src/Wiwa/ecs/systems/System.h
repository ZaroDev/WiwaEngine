#pragma once

#include <Wiwa/Application.h>
#include <Wiwa/ecs/EntityManager.h>

namespace Wiwa {
	template<class T, class T2, class... TArgs>
	class System {
	private:
		//std::vector<size_t> m_RemovedIndex;
		std::vector<EntityId> m_RegisteredEntities;
		
		std::vector<size_t> m_EntityComponentIndexes[sizeof...(TArgs) + 2];

		std::vector<byte*>* m_ECSComponents;
		//byte** m_SystemComponents[sizeof...(TArgs) + 2];

		ComponentId m_ComponentIds[sizeof...(TArgs) + 2];
		size_t m_ComponentSize[sizeof...(TArgs) + 2];

		size_t getEntityPos(EntityId eid);

		void AddEntity(EntityId entity);
		void RemoveEntity(size_t index);

		size_t m_ComponentCount;
		size_t m_ComponentIdCount;

		// Take component ids
		template<class C> void GetComponentECSIds();
		template<class C, class C2, class ...CArgs> void GetComponentECSIds();

		// Take component lists for this system
		/*template<class C> void GetSystemComponents();
		template<class C, class C2, class ...CArgs> void GetSystemComponents();*/

		
		void LoadComponents(size_t index);
		template<class C> void InsertComponents(size_t index);
		template<class C, class C2, class ...CArgs> void InsertComponents(size_t index);
		template<class C> size_t GetComponentIndex();

		// Component tuples
		template<class C> std::tuple<C*> GetComponentsTuple(size_t eindex, size_t cindex);
		template<class C, class C2, class ...CArgs> std::tuple<C*, C2*, CArgs*...> GetComponentsTuple(size_t eindex, size_t cindex);
	protected:
		// Callbacks for sub-systems
		virtual void OnEntityAdded(EntityId entityID) {}
		virtual void OnEntityRemoved() {}

		byte* GetComponents(size_t index);
		template<class C> C* GetComponents();
		size_t* GetComponentIndexes(size_t index);
		template<class C> size_t* GetComponentIndexes();
		size_t GetRegisteredSize() { return m_RegisteredEntities.size(); }

		virtual void OnInit() {}
		virtual void OnUpdate() {}

		virtual void OnUpdateComponents(T* arg1, T2* arg2, TArgs*... args) = 0;
	public:
		System();
		virtual ~System(); // Virtual destructor, so that child destructor is called

		void Reserve(size_t amount);

		bool OnEntityComponentAdded(EntityId entity);
		void OnEntityRemoved(EntityId entity);

		void Init();
		void Update();
	};

	//-------- CONSTRUCTOR --------
	template<class T, class T2, class ...TArgs>
	inline System<T, T2, TArgs...>::System() : m_ComponentIdCount(0)
	{
		m_ComponentCount = sizeof...(TArgs) + 2;

		Application& app = Application::Get();
		app.GetEntityManager().RegisterComponents<T, T2, TArgs...>();
		m_ECSComponents = app.GetEntityManager().GetComponentsList();

		GetComponentECSIds<T, T2, TArgs...>();
		//GetSystemComponents<T, T2, TArgs...>();
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
		size_t index = m_RegisteredEntities.size();

		m_RegisteredEntities.emplace_back(entity);

		for (size_t i = 0; i < m_ComponentCount; i++) {
			m_EntityComponentIndexes[i].emplace_back();
		}

		LoadComponents(index);
	
		// Callback for child system
		OnEntityAdded(entity);
	}

	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::RemoveEntity(size_t index)
	{
		m_RegisteredEntities.erase(m_RegisteredEntities.begin() + index);
		for (size_t i = 0; i < m_ComponentCount; i++) {
			m_EntityComponentIndexes[i].erase(m_EntityComponentIndexes[i].begin() + index);
		}

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

	//-------- Component Functions --------
	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::LoadComponents(size_t index)
	{
		Application& app = Application::Get();

		for (size_t i = 0; i < m_ComponentCount; i++) {
			m_EntityComponentIndexes[i][index] = app.GetEntityManager().GetComponentIndex(m_RegisteredEntities[index], m_ComponentIds[i], m_ComponentSize[i]);
		}
	}

	template<class T, class T2, class ...TArgs>
	inline byte* System<T, T2, TArgs...>::GetComponents(size_t index)
	{
		byte* c_list = NULL;

		if (index < m_ComponentCount) {
			c_list = m_ECSComponents->at(m_ComponentIds[index]);
		}

		return c_list;
	}

	template<class T, class T2, class ...TArgs>
	inline size_t* System<T, T2, TArgs...>::GetComponentIndexes(size_t index)
	{
		size_t* i_list = NULL;

		if (index < m_ComponentCount) {
			i_list = m_EntityComponentIndexes[index].data();
		}

		return i_list;
	}

	template<class T, class T2, class ...TArgs>
	inline void System<T, T2, TArgs...>::Reserve(size_t amount)
	{
		m_RegisteredEntities.reserve(amount);

		for (size_t i = 0; i < m_ComponentCount; i++) {
			m_EntityComponentIndexes[i].reserve(amount);
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

	/*template<class T, class T2, class ...TArgs>
	template<class C>
	inline void System<T, T2, TArgs...>::GetSystemComponents()
	{
		Application& app = Application::Get();

		size_t c_id = app.GetEntityManager().GetComponentId<C>();

		size_t c_i = GetComponentIndex<C>();

		m_SystemComponents[c_i] = app.GetEntityManager().GetComponentsPtr(c_id);
	}

	template<class T, class T2, class ...TArgs>
	template<class C, class C2, class ...CArgs>
	inline void System<T, T2, TArgs...>::GetSystemComponents()
	{
		GetSystemComponents<C>();
		GetSystemComponents<C2, CArgs...>();
	}*/

	template<class T, class T2, class ...TArgs>
	template<class C>
	inline void System<T, T2, TArgs...>::InsertComponents(size_t index)
	{
		size_t cid = GetComponentIndex<C>();

		Application& app = Application::Get();

		m_EntityComponentIndexes[cid][index] = app.GetEntityManager().GetComponentIndex(m_RegisteredEntities[index], m_ComponentIds[cid], m_ComponentSize[cid]);
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
	inline std::tuple<C*> System<T, T2, TArgs...>::GetComponentsTuple(size_t eindex, size_t cindex)
	{
		byte* cs = m_ECSComponents->at(m_ComponentIds[cindex]);
		size_t offset = m_EntityComponentIndexes[cindex][eindex];

		return std::make_tuple((C*)(cs + offset));
	}

	template<class T, class T2, class ...TArgs>
	template<class C, class C2, class ...CArgs>
	inline std::tuple<C*, C2*, CArgs*...> System<T, T2, TArgs...>::GetComponentsTuple(size_t eindex, size_t cindex)
	{
		return std::tuple_cat(GetComponentsTuple<C>(eindex, cindex), GetComponentsTuple<C2, CArgs...>(eindex, cindex + 1));
	}

	template<class T, class T2, class ...TArgs>
	template<class C>
	inline C* System<T, T2, TArgs...>::GetComponents()
	{
		size_t c_id = GetComponentIndex<C>();

		C* c_list = NULL;

		if (c_id < m_ComponentCount) {
			c_list = (C*)m_ECSComponents->at(m_ComponentIds[c_id]);
		}

		return c_list;
	}

	template<class T, class T2, class ...TArgs>
	template<class C>
	inline size_t* System<T, T2, TArgs...>::GetComponentIndexes()
	{
		size_t c_id = GetComponentIndex<C>();

		size_t* i_list = NULL;

		if (c_id < m_ComponentCount) {
			i_list = m_EntityComponentIndexes[c_id].data();
		}

		return i_list;
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
		//OnUpdate();

		size_t size = GetRegisteredSize();

		for (size_t i = 0; i < size; i++) {
			std::tuple<T*, T2*, TArgs*...> tuple = GetComponentsTuple<T, T2, TArgs...>(i, 0);
			std::apply(&System<T, T2, TArgs...>::OnUpdateComponents, std::tuple_cat(std::make_tuple(this), tuple));
		}
	}
}