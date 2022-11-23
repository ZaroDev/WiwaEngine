#include "wipch.h"

#include "EntityManager.h"
#include "systems/System.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/core/Renderer3D.h>

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
			delete m_Systems[i];
		}
	}

	void EntityManager::RemoveEntity(EntityId eid)
	{
		m_EntitiesRemoved.push_back(eid);

		// Remove entity from alive entities vector
		size_t ealive = m_EntitiesAlive.size();

		for (size_t i = 0; i < ealive; i++) {
			if (m_EntitiesAlive[i] == eid) {
				m_EntitiesAlive.erase(m_EntitiesAlive.begin() + i);
				break;
			}
		}
		
		// Remove entity from parent alive entities vector
		if (m_EntityParent[eid] == eid) {
			size_t pealive = m_ParentEntitiesAlive.size();

			for (size_t i = 0; i < pealive; i++) {
				if (m_ParentEntitiesAlive[i] == eid) {
					m_ParentEntitiesAlive.erase(m_ParentEntitiesAlive.begin() + i);
					break;
				}
			}
		}

		// Callback for systems
		size_t systems = m_EntitySystems[eid].size();

		for (size_t i = 0; i < systems; i++) {
			m_Systems[i]->RemoveEntity(eid);
		}

		m_EntitySystems[eid].clear();

		// Add to removed component indexes
		std::map<ComponentId, size_t>::iterator c_it;

		for (c_it = m_EntityComponents[eid].begin(); c_it != m_EntityComponents[eid].end(); c_it++) {
			m_ComponentsRemoved[c_it->first].push_back(c_it->second);
		}

		m_EntityComponents[eid].clear();

		// Remove children entities
		size_t chsize = m_EntityChildren[eid].size();

		for (size_t i = 0; i < chsize; i++) {
			RemoveEntity(m_EntityChildren[eid][i]);
		}

		m_EntityChildren[eid].clear();
	}

	void EntityManager::Update()
	{
		// Remove entities in pool
		size_t rsize = m_EntitiesToDestroy.size();

		for (size_t i = 0; i < rsize; i++) {
			RemoveEntity(m_EntitiesToDestroy[i]);
		}

		m_EntitiesToDestroy.clear();

		// Update systems
		size_t sysize = m_Systems.size();

		for (size_t i = 0; i < sysize; i++) {
			m_Systems[i]->Update();
		}
	}

	EntityId EntityManager::CreateEntity_impl()
	{
		size_t rsize = m_EntitiesRemoved.size();

		EntityId eid = 0;

		if (rsize > 0) {
			eid = m_EntitiesRemoved[rsize - 1];

			m_EntitiesRemoved.pop_back();
		}
		else {
			eid = m_EntityComponents.size();

			m_EntityComponents.emplace_back();
			m_EntitySystems.emplace_back();
			m_EntityNames.emplace_back();
			m_EntityParent.emplace_back();
			m_EntityChildren.emplace_back();
		}

		m_EntitiesAlive.push_back(eid);

		return eid;
	}

	EntityId EntityManager::CreateEntity()
	{
		return CreateEntity("New entity");
	}

	EntityId EntityManager::CreateEntity(const char* name)
	{
		EntityId eid = CreateEntity_impl();

		m_EntityNames[eid] = name;
		m_EntityParent[eid] = eid;

		m_ParentEntitiesAlive.emplace_back(eid);

		return eid;
	}

	EntityId EntityManager::CreateEntity(EntityId parent)
	{
		return CreateEntity("New entity", parent);
	}

	EntityId EntityManager::CreateEntity(const char* name, EntityId parent)
	{
		EntityId eid = CreateEntity_impl();

		m_EntityNames[eid] = name;
		m_EntityParent[eid] = parent;
		m_EntityChildren[parent].push_back(eid);

		return eid;
	}

	void EntityManager::DestroyEntity(EntityId entity)
	{
		m_EntitiesToDestroy.push_back(entity);
	}

	void EntityManager::ReserveEntities(size_t amount)
	{
		m_EntityComponents.reserve(amount);
		m_EntitySystems.reserve(amount);
		m_EntityParent.reserve(amount);
		m_EntityChildren.reserve(amount);
		m_EntitiesAlive.reserve(amount);
		m_EntitiesRemoved.reserve(amount);
		m_ParentEntitiesAlive.reserve(amount);
	}

	void EntityManager::ReserveComponent(ComponentHash hash, size_t amount)
	{
		// Get component ID
		ComponentId cid = GetComponentId(hash);
		const Type* c_type = m_ComponentIds[cid].ctype;
		size_t c_size = c_type->size;

		if (cid >= m_Components.size()) {
			m_Components.resize(cid + 1, NULL);
			m_ComponentsSize.resize(cid + 1, 0);
			m_ComponentsReserved.resize(cid + 1, 0);
			m_ComponentTypes.resize(cid + 1, NULL);
			m_ComponentsRemoved.resize(cid + 1);
		}

		if (m_Components[cid]) {
			byte* oldBlock = m_Components[cid];
			size_t oldSize = m_ComponentsSize[cid] * c_size;
			size_t newSize = oldSize + amount * c_size;

			byte* newBlock = new byte[newSize];

			memcpy(newBlock, oldBlock, oldSize);

			delete[] oldBlock;

			m_Components[cid] = newBlock;
		}
		else {
			m_Components[cid] = new byte[amount * c_size];
			m_ComponentTypes[cid] = c_type;
		}

		m_ComponentsReserved[cid] = m_ComponentsSize[cid] + amount;
	}

	byte* EntityManager::AddComponent(EntityId entityId, ComponentHash hash, byte* value) {
		const Type* ctype = Application::Get().getCoreTypeH(hash);
		if (!ctype) ctype = Application::Get().getAppTypeH(hash);

		return AddComponent(entityId, ctype, value);
	}

	byte* EntityManager::AddComponent(EntityId entity, const Type* type, byte* value)
	{
		// Get component ID
		ComponentId cid = GetComponentId(type);

		// Take components map of the entity
		std::map<ComponentId, size_t>* ec = &m_EntityComponents[entity];

		byte* component = NULL;

		// Look if the entity has this component
		std::map<ComponentId, size_t>::iterator iterator = ec->find(cid);

		// If it doesn't have the component, add it
		if (iterator == ec->end()) {
			size_t t_size = type->size;
			// Check if the component list exists
			if (cid >= m_Components.size()) {
				m_Components.resize(cid + 1, NULL);
				m_ComponentsSize.resize(cid + 1, 0);
				m_ComponentsReserved.resize(cid + 1, 0);
				m_ComponentTypes.resize(cid + 1, NULL);
				m_ComponentsRemoved.resize(cid + 1);
			}

			// If it's the first component, create new block MARTA WAS HERE
			if (!m_Components[cid]) {
				byte* block = new byte[t_size];

				component = block;//new(block) byte[t_size]{ 0 };

				if (value) memcpy(component, value, t_size);
				else memset(component, 0, t_size);

				m_Components[cid] = block;
				m_ComponentsSize[cid]++;
				m_ComponentsReserved[cid]++;
				ec->insert_or_assign(cid, 0);

				m_ComponentTypes[cid] = type;
			}
			else {
				// Check if component index available
				size_t c_rsize = m_ComponentsRemoved[cid].size();

				// If index is available, construct
				if (c_rsize > 0) {
					size_t c_index = m_ComponentsRemoved[cid][c_rsize - 1];

					byte* components = m_Components[cid];

					size_t ind = c_index * t_size;

					component = components + ind;//new(&components[ind]) byte[t_size]{ 0 };

					if (value) memcpy(component, value, t_size);
					else memset(component, 0, t_size);

					m_ComponentsRemoved[cid].pop_back();

					ec->insert_or_assign(cid, c_index);
				}
				// If not, proceed normally
				else {
					// If more components reserved, just construct me TOO MEEEEEEEEEEEEE :) :3 "^0^" ÙwÚ
					if (m_ComponentsSize[cid] < m_ComponentsReserved[cid]) {
						byte* components = m_Components[cid];

						size_t last = m_ComponentsSize[cid] * t_size;

						component = components + last;//new(&components[last]) byte[t_size]{ 0 };

						if (value) memcpy(component, value, t_size);
						else memset(component, 0, t_size);
					}
					// If not, expand the block and construct
					else {
						byte* oldBlock = m_Components[cid];
						size_t oldSize = m_ComponentsSize[cid] * t_size;
						size_t newSize = oldSize + t_size;

						byte* newBlock = new byte[newSize];

						memcpy(newBlock, oldBlock, oldSize);

						delete[] oldBlock;

						component = newBlock + oldSize;//new(&newBlock[oldSize]) byte[t_size]{ 0 };

						if (value) memcpy(component, value, t_size);
						else memset(component, 0, t_size);

						m_Components[cid] = newBlock;

						m_ComponentsReserved[cid]++;
					}

					// Update entity-component map and components size/reserved
					ec->insert_or_assign(cid, m_ComponentsSize[cid]);
					m_ComponentsSize[cid]++;
				}
			}
		}

		return component;
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

	size_t EntityManager::GetComponentIndex(EntityId entityId, ComponentId componentId, size_t componentSize)
	{
		size_t index = -1;

		if (entityId < m_EntityComponents.size()) {
			std::map<ComponentId, size_t>::iterator it = m_EntityComponents[entityId].find(componentId);

			if (it != m_EntityComponents[entityId].end()) {
				index = it->second;
			}
		}

		return index;
	}

	ComponentId EntityManager::GetComponentId(const Type* type)
	{
		size_t component_id = 0;

		std::unordered_map<size_t, componentData>::iterator cid = m_ComponentIds.find(type->hash);

		if (cid == m_ComponentIds.end()) {
			component_id = m_ComponentIdCount++;

			m_ComponentIds[type->hash] = { type, component_id };
		}
		else {
			component_id = cid->second.cid;
		}

		return component_id;
	}

	ComponentId EntityManager::GetComponentId(ComponentHash hash) {
		const Type* ctype = Application::Get().getCoreTypeH(hash);
		if (!ctype) ctype = Application::Get().getAppTypeH(hash);

		return GetComponentId(ctype);
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

	void EntityManager::ApplySystem(EntityId eid, SystemId sid)
	{
		m_Systems[sid]->AddEntity(eid);
	}
}