#pragma once
#include <Wiwa/Core.h>
#include <Wiwa/Layer.h>

#include <vector>
#include <map>
#include <unordered_map>

#include <Wiwa/Reflection.h>
#include <Wiwa/utilities/Action.h>

typedef size_t EntityId;
typedef size_t ComponentId;
typedef size_t SystemId;
typedef unsigned char byte;

namespace Wiwa {
	class WI_API EntityManager : public Layer
	{
	private:
		struct componentData {
			const Type* ctype;
			ComponentId cid;
		};

		// Component variables
		size_t m_ComponentIdCount;

		std::vector<byte*> m_Components;
		std::unordered_map<size_t, componentData> m_ComponentIds;
		std::vector<size_t> m_ComponentsSize;
		std::vector<size_t> m_ComponentsReserved;
		std::vector<std::map<ComponentId, size_t>> m_EntityComponents;
		std::vector<std::vector<SystemId>> m_EntitySystems;

		template<class T> bool HasComponents(EntityId entityId);

		// System variables
		std::unordered_map<size_t, SystemId> m_SystemIds;
		size_t m_SystemIdCount;

		std::vector<void*> m_Systems;

		void OnEntityComponentAdded(EntityId eid);
	public:
		EntityManager();
		~EntityManager();

		// System registration functions
		//Action<> registrations[10];

		// Module functions
		void OnUpdate() override;

		// Create entity
		EntityId CreateEntity();

		// Component functions
		template<class T> T* AddComponent(EntityId entity, T value = {});
		template<class T> T* GetComponents(size_t* size);

		template<class T> void AddComponents(EntityId entity, T arg = {});
		template<class T, class T2, class... TArgs> void AddComponents(EntityId entity, T arg1, T2 arg2, TArgs... args);
		template<class T, class T2, class... TArgs> void AddComponents(EntityId entity);

		void ReserveEntities(size_t amount);
		template<class T> void ReserveComponent(size_t amount);

		byte* GetComponent(EntityId entityId, ComponentId componentId, size_t componentSize);
		template<class T> T* GetComponent(EntityId entityId);

		template<class T> bool HasComponent(EntityId entityId);

		bool HasComponents(EntityId entityId, ComponentId* componentIds, size_t size);
		template<class T, class T2, class... TArgs> bool HasComponents(EntityId entityId);
		
		template<class T> ComponentId GetComponentId();

		// System functions
		bool HasSystem(EntityId eid, SystemId sid);

		template<class T> SystemId GetSystemId();

		template<class T> void RegisterSystem();
		template<class T> void ReserveSystem(size_t amount);
	};

	// Get system ID using Reflection
	template<class T>
	inline SystemId EntityManager::GetSystemId()
	{
		size_t system_id = 0;

		const Type* stype = GetType<T>();
		std::unordered_map<size_t, SystemId>::iterator sid = m_SystemIds.find(stype->hash);

		if (sid == m_SystemIds.end()) {
			system_id = m_SystemIdCount++;

			m_SystemIds[stype->hash] = system_id;
		}
		else {
			system_id = sid->second;
		}

		return system_id;
	}

	// Get component ID using Reflection
	template<class T>
	inline ComponentId EntityManager::GetComponentId()
	{
		size_t component_id = 0;

		const Type* ctype = GetType<T>();
		std::unordered_map<size_t, componentData>::iterator cid = m_ComponentIds.find(ctype->hash);

		if (cid == m_ComponentIds.end()) {
			component_id = m_ComponentIdCount++;

			m_ComponentIds[ctype->hash] = { GetType<T>(), component_id };
		}
		else {
			component_id = cid->second.cid;
		}

		return component_id;
	}

	template<class T>
	inline T* EntityManager::AddComponent(EntityId entity, T value)
	{
		// Get component ID
		ComponentId cid = GetComponentId<T>();

		// Take components map of the entity
		std::map<ComponentId, size_t>* ec = &m_EntityComponents[entity];

		T* component = NULL;

		// Look if the entity has this component
		std::map<ComponentId, size_t>::iterator iterator = ec->find(cid);

		// If it doesn't have the component, add it
		if (iterator == ec->end()) {
			size_t t_size = sizeof(T);
			// Check if the component list exists
			if (cid >= m_Components.size()) {
				m_Components.resize(cid + 1, NULL);
				m_ComponentsSize.resize(cid + 1, 0);
				m_ComponentsReserved.resize(cid + 1, 0);
			}

			// If it's the first component, create new block MARTA WAS HERE
			if (!m_Components[cid]) {
				byte* block = new byte[t_size];

				component = new(block) T(value);

				m_Components[cid] = block;
				m_ComponentsSize[cid]++;
				ec->insert_or_assign(cid, 0);
			}
			else {
				// If more components reserved, just construct me TOO MEEEEEEEEEEEEE :) :3 "^0^" ÙwÚ
				if (m_ComponentsSize[cid] < m_ComponentsReserved[cid]) {
					byte* components = m_Components[cid];

					size_t last = m_ComponentsSize[cid] * t_size;

					component = new(&components[last]) T(value);
				}
				// If not, expand the block and construct
				else {
					byte* oldBlock = m_Components[cid];
					size_t oldSize = m_ComponentsSize[cid] * t_size;
					size_t newSize = oldSize + t_size;

					byte* newBlock = new byte[newSize];

					memcpy(newBlock, oldBlock, oldSize);

					delete[] oldBlock;

					component = new(&newBlock[oldSize]) T(value);

					m_Components[cid] = newBlock;
				}

				// Update entity-component map and components size/reserved
				ec->insert_or_assign(cid, m_ComponentsSize[cid]);
				m_ComponentsSize[cid]++;
				m_ComponentsReserved[cid]++;
			}
		}
		// If it does have the component, return it
		else {
			component = (T*)&m_Components[cid][iterator->second * sizeof(T)];
		}

		// Callbacks for systems
		OnEntityComponentAdded(entity);

		return component;
	}

	template<class T>
	inline T* EntityManager::GetComponents(size_t* size)
	{
		// Get component ID
		ComponentId cid = GetComponentId<T>();

		byte* components = NULL;
		*size = 0;

		if (cid < m_Components.size()) {
			*size = m_ComponentsSize[cid];
			components = m_Components[cid];
		}

		return (T*)(void*)components;
	}

	template<class T>
	inline void EntityManager::AddComponents(EntityId entity, T arg)
	{
		AddComponent(entity, arg);
	}

	template<class T, class T2, class ...TArgs>
	inline void EntityManager::AddComponents(EntityId entity, T arg1, T2 arg2, TArgs ...args)
	{
		AddComponents<T>(entity, arg1);
		AddComponents<T2, TArgs...>(entity, arg2, args...);
	}

	template<class T, class T2, class ...TArgs>
	inline void EntityManager::AddComponents(EntityId entity)
	{
		AddComponents<T>(entity);
		AddComponents<T2, TArgs...>(entity);
	}

	template<class T>
	inline void EntityManager::ReserveComponent(size_t amount)
	{
		// Get component ID
		ComponentId cid = GetComponentId<T>();

		if (cid >= m_Components.size()) {
			m_Components.resize(cid + 1, NULL);
			m_ComponentsSize.resize(cid + 1, 0);
			m_ComponentsReserved.resize(cid + 1, 0);
		}

		if (m_Components[cid]) {
			byte* oldBlock = m_Components[cid];
			size_t oldSize = m_ComponentsSize[cid] * sizeof(T);
			size_t newSize = oldSize + amount * sizeof(T);

			byte* newBlock = new byte[newSize];

			memcpy(newBlock, oldBlock, oldSize);

			delete[] oldBlock;

			m_Components[cid] = newBlock;
		}
		else {
			m_Components[cid] = new byte[amount * sizeof(T)];
		}

		m_ComponentsReserved[cid] = m_ComponentsSize[cid] + amount;
	}

	template<class T>
	inline T* EntityManager::GetComponent(EntityId entityId)
	{
		T* c = NULL;

		if (entityId < m_EntityComponents.size()) {
			ComponentId cid = GetComponentId<T>();

			std::map<ComponentId, size_t>::iterator it = m_EntityComponents[entityId].find(cid);

			if (it != m_EntityComponents[entityId].end()) {
				c = (T*)&m_Components[cid][it->second * sizeof(T)];
			}
		}

		return c;
	}

	template<class T>
	inline bool EntityManager::HasComponent(EntityId entityId)
	{
		if (entityId >= m_EntityComponents.size()) {
			return false;
		}

		ComponentId cid = GetComponentId<T>();

		std::map<ComponentId, size_t>* emap = &m_EntityComponents[entityId];

		std::map<ComponentId, size_t>::iterator it = emap->find(cid);

		return it != emap->end();
	}

	template<class T>
	inline bool EntityManager::HasComponents(EntityId entityId)
	{
		return HasComponent<T>(entityId);
	}

	template<class T, class T2, class ...TArgs>
	inline bool EntityManager::HasComponents(EntityId entityId)
	{
		return HasComponents<T>(entityId) && HasComponents<T2, TArgs...>(entityId);
	}

	template<class T>
	inline void EntityManager::RegisterSystem()
	{
		SystemId sid = GetSystemId<T>();

		if (sid >= m_Systems.size()) {
			m_Systems.resize(sid + 1, NULL);
		}

		if (m_Systems[sid] == NULL) {
			m_Systems[sid] = new T();
		}
	}

	template<class T>
	inline void EntityManager::ReserveSystem(size_t amount)
	{
		SystemId sid = GetSystemId<T>();

		if (sid < m_Systems.size()) {
			((T*)m_Systems[sid])->Reserve(amount);
		}
	}
}