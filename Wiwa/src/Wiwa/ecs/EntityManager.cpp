#include "wipch.h"

#include "EntityManager.h"
#include "systems/System.h"

#include <Wiwa/Application.h>

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

	void EntityManager::AddComponent(EntityId entityId, ComponentHash hash) {
		// Get component ID
		ComponentId cid = GetComponentId(hash);

		// Take components map of the entity
		std::map<ComponentId, size_t>* ec = &m_EntityComponents[entityId];

		const Type* ctype = Application::Get().getTypeH(hash);
		byte* component = NULL;

		// Look if the entity has this component
		std::map<ComponentId, size_t>::iterator iterator = ec->find(cid);

		// If it doesn't have the component, add it
		if (iterator == ec->end()) {
			size_t t_size = ctype->size;
			// Check if the component list exists
			if (cid >= m_Components.size()) {
				m_Components.resize(cid + 1, NULL);
				m_ComponentsSize.resize(cid + 1, 0);
				m_ComponentsReserved.resize(cid + 1, 0);
			}

			// If it's the first component, create new block MARTA WAS HERE
			if (!m_Components[cid]) {
				byte* block = new byte[t_size];

				component = new(block) byte[t_size]{ 0 };

				m_Components[cid] = block;
				m_ComponentsSize[cid]++;
				ec->insert_or_assign(cid, 0);

				m_ComponentTypes.push_back(ctype);
			}
			else {
				// If more components reserved, just construct me TOO MEEEEEEEEEEEEE :) :3 "^0^" ÙwÚ
				if (m_ComponentsSize[cid] < m_ComponentsReserved[cid]) {
					byte* components = m_Components[cid];

					size_t last = m_ComponentsSize[cid] * t_size;

					component = new(&components[last]) byte[t_size]{ 0 };
				}
				// If not, expand the block and construct
				else {
					byte* oldBlock = m_Components[cid];
					size_t oldSize = m_ComponentsSize[cid] * t_size;
					size_t newSize = oldSize + t_size;

					byte* newBlock = new byte[newSize];

					memcpy(newBlock, oldBlock, oldSize);

					delete[] oldBlock;

					component = new(&newBlock[oldSize]) byte[t_size]{ 0 };

					m_Components[cid] = newBlock;
				}

				// Update entity-component map and components size/reserved
				ec->insert_or_assign(cid, m_ComponentsSize[cid]);
				m_ComponentsSize[cid]++;
				m_ComponentsReserved[cid]++;
			}
		}

		// Callbacks for systems
		OnEntityComponentAdded(entityId);
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

	ComponentId EntityManager::GetComponentId(ComponentHash hash) {
		size_t component_id = 0;

		std::unordered_map<size_t, componentData>::iterator cid = m_ComponentIds.find(hash);

		if (cid == m_ComponentIds.end()) {
			component_id = m_ComponentIdCount++;

			m_ComponentIds[hash] = { Application::Get().getTypeH(hash), component_id};
		}
		else {
			component_id = cid->second.cid;
		}

		return component_id;
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