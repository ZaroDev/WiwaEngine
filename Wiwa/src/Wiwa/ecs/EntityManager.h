#pragma once
#pragma warning(disable : 4251)
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Layer.h>

#include <vector>
#include <map>
#include <unordered_map>

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/functions/Action.h>

typedef size_t EntityId;
typedef size_t ComponentId;
typedef size_t ComponentHash;
typedef size_t SystemHash;

typedef unsigned char byte;

namespace Wiwa {
	class System;

	struct Transform3D;

	class WI_API EntityManager
	{
	private:
		struct componentData {
			const Type* ctype;
			ComponentId cid;
		};

		// Entity management
		std::vector<std::string> m_EntityNames;
		std::vector<std::map<ComponentId, size_t>> m_EntityComponents;
		std::vector<std::vector<SystemHash>> m_EntitySystemHashes;
		std::vector<std::vector<System*>> m_EntitySystems;
		std::vector<EntityId> m_EntityParent;
		std::vector<bool> m_EntityActive;
		std::vector<std::vector<EntityId>> m_EntityChildren;

		std::vector<EntityId> m_EntitiesRemoved;
		std::vector<EntityId> m_EntitiesAlive;
		std::vector<EntityId> m_ParentEntitiesAlive;
		std::vector<EntityId> m_EntitiesToDestroy;

		// Create an entity and return its ID
		EntityId CreateEntity_impl();

		// Component management
		size_t m_ComponentIdCount;
		std::vector<std::vector<size_t>> m_ComponentsRemoved;
		std::vector<byte*> m_Components;
		std::vector<const Type*> m_ComponentTypes;
		std::unordered_map<ComponentHash, componentData> m_ComponentIds;
		std::vector<size_t> m_ComponentsSize;
		std::vector<size_t> m_ComponentsReserved;
		
		template<class T> bool HasComponents(EntityId entityId);

		void RemoveEntity(EntityId eid);

		void UpdateChildTransforms(EntityId eid, Transform3D* t3dparent);
		void UpdateTransforms();

		// System management
		std::unordered_map<SystemHash, std::vector<System*>> m_SystemsByHash;
		std::vector<SystemHash> m_SystemWhiteList;

		size_t getSystemIndex(EntityId entityId, SystemHash system_hash);
	public:
		EntityManager();
		~EntityManager();

		static const size_t INVALID_INDEX = -1;

		// System registration functions
		//Action<> registrations[10];

		// Systems awake
		void SystemsAwake();

		// Systems init
		void SystemsInit();

		// Systems update
		void SystemsUpdate();

		// Update entity manager
		void Update();

		// Update whitelist
		void UpdateWhitelist();

		// Add system to whitelist
		void AddSystemToWhitelist(SystemHash system_hash);

		// Add system to whitelist by name
		void AddSystemToWhitelist(const char* system_name);

		// Add system to whitelist by type
		template<class T> void AddSystemToWhitelist();

		// Checks if a system is whitelisted
		bool IsWhitelistedSystem(SystemHash system_hash);

		// Create entity
		EntityId CreateEntity();
		EntityId CreateEntity(const char* name);
		EntityId CreateEntity(EntityId parent);
		EntityId CreateEntity(const char* name, EntityId parent);

		void SetParent(EntityId entity, EntityId parent);

		bool IsParent(EntityId entity) { return m_EntityParent[entity] == entity; }

		void SetActive(EntityId entity, bool active) { m_EntityActive[entity] = active; }

		bool IsActive(EntityId entity) { return m_EntityActive[entity]; }

		// Remove entity
		void DestroyEntity(EntityId entity);

		// Entity functions
		inline const char* GetEntityName(EntityId id) { return m_EntityNames[id].c_str(); }
		inline void SetEntityName(EntityId id, const char* name) { m_EntityNames[id] = name; }

		inline std::vector<EntityId>* GetEntitiesAlive() { return &m_EntitiesAlive; }
		inline std::vector<EntityId>* GetParentEntitiesAlive() { return &m_ParentEntitiesAlive; }

		inline size_t GetEntityCount() { return m_EntitiesAlive.size(); }
		inline EntityId GetEntityParent(EntityId eid) { return m_EntityParent[eid]; }
		inline std::vector<EntityId>* GetEntityChildren(EntityId eid) { return &m_EntityChildren[eid]; }

		inline std::map<ComponentId, size_t>& GetEntityComponents(EntityId id) { return m_EntityComponents[id]; }

		inline std::vector<SystemHash>& GetEntitySystemHashes(EntityId id) { return m_EntitySystemHashes[id]; }
		
		inline const Type* GetComponentType(ComponentId id) { return m_ComponentTypes[id]; }

		size_t GetComponentIndex(EntityId entityId, ComponentId componentId, size_t componentSize);

		// Add component by hash
		byte* AddComponent(EntityId entity, ComponentHash hash, byte* value=NULL);

		// Add component by type
		byte* AddComponent(EntityId entity, const Type* type, byte* value = NULL);

		// Add component by template
		template<class T> T* AddComponent(EntityId entity, T value = {});

		template<class T> void AddComponents(EntityId entity, T arg = {});
		template<class T, class T2, class... TArgs> void AddComponents(EntityId entity, T arg1, T2 arg2, TArgs... args);
		template<class T, class T2, class... TArgs> void AddComponents(EntityId entity);

		void RemoveComponent(EntityId entity, ComponentHash hash);
		void RemoveComponentById(EntityId entity, ComponentId componentId);

		template<class T> void RemoveComponent(EntityId entity);

		// Component get functions
		byte* GetComponent(EntityId entityId, ComponentId componentId, size_t componentSize);
		template<class T> T* GetComponent(EntityId entityId);

		inline byte** GetComponentsPtr(ComponentId id) { return &m_Components[id]; }
		inline byte* GetComponents(ComponentId id) { return m_Components[id]; }
		template<class T> T* GetComponents(size_t* size);

		inline std::vector<byte*>* GetComponentsList() { return &m_Components; }

		// Register components (called on system register)
		template<class T> void RegisterComponent();

		template<class T> void RegisterComponents();
		template<class T, class T2, class ...TArgs> void RegisterComponents();

		template<class T> byte* GetComponents();

		// Reserve functions
		void ReserveEntities(size_t amount);
		void ReserveComponent(ComponentHash hash, size_t amount);
		template<class T> void ReserveComponent(size_t amount);

		// Component utilities
		template<class T> bool HasComponent(EntityId entityId);

		bool HasComponents(EntityId entityId, ComponentId* componentIds, size_t size);
		template<class T, class T2, class... TArgs> bool HasComponents(EntityId entityId);
		
		ComponentId GetComponentId(const Type* type);
		ComponentId GetComponentId(ComponentHash hash);
		template<class T> ComponentId GetComponentId();

		// System functions
		bool HasSystem(EntityId eid, SystemHash sid);

		// Register systems
		template<class T> void ApplySystem(EntityId eid);
		void ApplySystem(EntityId eid, SystemHash system_hash);
		void ApplySystem(EntityId eid, const Type* system_type);

		// Remove system from entity
		void RemoveSystem(EntityId eid, SystemHash system_hash);
	};

	// Get component ID using Reflection
	template<class T>
	inline ComponentId EntityManager::GetComponentId()
	{
		const Type* ctype = GetType<T>();

		return GetComponentId(ctype);
	}

	template<class T>
	inline T* EntityManager::AddComponent(EntityId entity, T value)
	{
		const Type* type = GetType<T>();

		byte* data = (byte*)&value;

		T* component = (T*)AddComponent(entity, type, data);

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
	inline void EntityManager::RegisterComponent()
	{
		ComponentId cid = GetComponentId<T>();

		if (cid >= m_Components.size()) {
			m_Components.resize(cid + 1, NULL);
			m_ComponentsSize.resize(cid + 1, 0);
			m_ComponentsReserved.resize(cid + 1, 0);
			m_ComponentTypes.resize(cid + 1, NULL);
			m_ComponentsRemoved.resize(cid + 1);
		}
	}

	template<class T>
	inline void EntityManager::RegisterComponents()
	{
		RegisterComponent<T>();
	}

	template<class T, class T2, class ...TArgs>
	inline void EntityManager::RegisterComponents()
	{
		RegisterComponents<T>();
		RegisterComponents<T2, TArgs...>();
	}

	template<class T>
	inline byte* EntityManager::GetComponents()
	{
		ComponentId cid = GetComponentId<T>();

		byte* components = NULL;

		if (cid < m_Components.size()) {
			components = m_Components[cid];
		}

		return components;;
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
			m_ComponentTypes.resize(cid + 1, NULL);
			m_ComponentsRemoved.resize(cid + 1);
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
			m_ComponentTypes[cid] = GetType<T>();
		}

		m_ComponentsReserved[cid] = m_ComponentsSize[cid] + amount;
	}

	template<class T>
	inline void EntityManager::RemoveComponent(EntityId entity)
	{
		const Type* t = GetType<T>();

		RemoveComponent(entity, t->hash);
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

	template<class T>
	inline void EntityManager::AddSystemToWhitelist()
	{
		const Type* stype = GetType<T>();

		assert(stype->custom_id == 1);

		AddSystemToWhitelist(stype->hash);
	}

	template<class T, class T2, class ...TArgs>
	inline bool EntityManager::HasComponents(EntityId entityId)
	{
		return HasComponents<T>(entityId) && HasComponents<T2, TArgs...>(entityId);
	}

	template<class T>
	inline void EntityManager::ApplySystem(EntityId eid)
	{
		const Type* stype = GetType<T>();

		assert(stype->custom_id == 1);

		ApplySystem(eid, stype->hash);
	}
}