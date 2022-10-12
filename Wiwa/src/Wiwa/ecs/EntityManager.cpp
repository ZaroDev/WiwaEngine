#include "wipch.h"

#include "EntityManager.h"
#include "systems/System.h"

namespace Wiwa {
	EntityManager::EntityManager()
	{
		m_ComponentIdCount = 0;
		m_SystemIdCount = 0;
	}

	EntityManager::~EntityManager()
	{
		size_t size = m_Systems.size();

		for (size_t i = 0; i < size; i++) {
			System<void, void>* s = (System<void, void>*)m_Systems[i];

			delete s;
		}
	}

	void EntityManager::OnEntityComponentAdded(EntityId eid)
	{
		size_t size = m_Systems.size();

		for (size_t i = 0; i < size; i++) {
			if (!HasSystem(eid, i)) {
				System<void, void>* s = (System<void, void>*)m_Systems[i];

				if (s->OnEntityComponentAdded(eid)) {
					m_EntitySystems[eid].push_back(i);
				}
			}
		}
	}

	void EntityManager::OnUpdate()
	{
		size_t size = m_Systems.size();

		for (size_t i = 0; i < size; i++) {
			System<void, void>* s = (System<void, void>*)m_Systems[i];

			s->Update();
		}
	}

	EntityId EntityManager::CreateEntity()
	{
		m_EntityComponents.emplace_back();
		m_EntitySystems.emplace_back();
		m_EntityNames.emplace_back("New entity");
		return m_EntityComponents.size() - 1;
	}

	EntityId EntityManager::CreateEntity(const char* name)
	{
		m_EntityNames.emplace_back(name);
		m_EntityComponents.emplace_back();
		m_EntitySystems.emplace_back();
		return m_EntityComponents.size() - 1;
	}

	void EntityManager::ReserveEntities(size_t amount)
	{
		m_EntityComponents.reserve(amount);
		m_EntitySystems.reserve(amount);
	}

	byte* EntityManager::GetComponent(EntityId entityId, ComponentId componentId, size_t componentSize)
	{
		byte* c = NULL;

		if (entityId < m_EntityComponents.size()) {
			std::map<ComponentId, size_t>::iterator it = m_EntityComponents[entityId].find(componentId);

			if (it != m_EntityComponents[entityId].end()) {
				c = &m_Components[componentId][it->second * componentSize];
			}
		}

		return c;
	}

	bool EntityManager::HasComponents(EntityId entityId, ComponentId* componentIds, size_t size)
	{
		bool hasComponents = true;

		if (entityId >= m_EntityComponents.size()) {
			return false;
		}

		std::map<ComponentId, size_t>* emap = &m_EntityComponents[entityId];

		for (size_t i = 0; i < size && hasComponents; i++) {
			hasComponents = hasComponents && emap->find(componentIds[i]) != emap->end();
		}

		return hasComponents;
	}
	bool EntityManager::HasSystem(EntityId eid, SystemId sid)
	{
		size_t size = m_EntitySystems[eid].size();

		for (size_t i = 0; i < size; i++) {
			if (m_EntitySystems[eid][i] == sid)
				return true;
		}

		return false;
	}
}