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
	}

	EntityManager::~EntityManager()
	{

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
		
		EntityId p_ent = m_EntityParent[eid];

		// Remove entity from parent alive entities vector
		if (p_ent == eid) {
			size_t pealive = m_ParentEntitiesAlive.size();

			for (size_t i = 0; i < pealive; i++) {
				if (m_ParentEntitiesAlive[i] == eid) {
					m_ParentEntitiesAlive.erase(m_ParentEntitiesAlive.begin() + i);
					break;
				}
			}
		}
		// Remove entity from parent's child list
		else {
			std::vector<EntityId>& p_entities = m_EntityChildren[p_ent];
			size_t p_entities_size = p_entities.size();

			for (size_t i = 0; i < p_entities_size; i++) {
				if (p_entities[i] == eid) {
					p_entities.erase(p_entities.begin() + i);
					break;
				}
			}
		}

		// Callback for systems
		size_t systems_size = m_EntitySystems[eid].size();

		for (size_t i = 0; i < systems_size; i++) {
			System* system = m_EntitySystems[eid][i];
			system->Destroy();

			delete system;
		}

		m_EntitySystemIds[eid].clear();
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
			RemoveEntity(m_EntityChildren[eid][0]);
		}

		m_EntityChildren[eid].clear();
	}

	void EntityManager::UpdateChildTransforms(EntityId eid, Transform3D* t3dparent)
	{
		Transform3D* t3d = GetComponent<Transform3D>(eid);
		
		// Update transforms
		t3d->position = t3dparent->position + t3d->localPosition;
		t3d->rotation = t3dparent->rotation + t3d->localRotation;
		t3d->scale = t3dparent->scale * t3d->localScale;

		// Update children
		std::vector<EntityId>& children = m_EntityChildren[eid];
		size_t c_size = children.size();

		for (size_t i = 0; i < c_size; i++) {
			UpdateChildTransforms(children[i], t3d);
		}
	}

	void EntityManager::UpdateTransforms()
	{
		size_t p_ent_size = m_ParentEntitiesAlive.size();

		for (size_t i = 0; i < p_ent_size; i++) {
			EntityId p_ent = m_ParentEntitiesAlive[i];

			Transform3D* t3d = GetComponent<Transform3D>(p_ent);

			t3d->position = t3d->localPosition;
			t3d->rotation = t3d->localRotation;
			t3d->scale = t3d->localScale;

			// Update children
			std::vector<EntityId>& children = m_EntityChildren[p_ent];
			size_t c_size = children.size();

			for (size_t i = 0; i < c_size; i++) {
				UpdateChildTransforms(children[i], t3d);
			}
		}
	}

	void EntityManager::Update()
	{
		// Remove entities in pool
		size_t rsize = m_EntitiesToDestroy.size();

		for (size_t i = 0; i < rsize; i++) {
			RemoveEntity(m_EntitiesToDestroy[i]);
		}

		m_EntitiesToDestroy.clear();

		// Update transforms
		UpdateTransforms();

		// Update entity systems
		size_t entitySize = m_EntitySystems.size();

		for (size_t i = 0; i < entitySize; i++) {
			size_t system_size = m_EntitySystems[i].size();

			for (size_t j = 0; j < system_size; j++) {
				System* s = m_EntitySystems[i][j];

				s->Update();
			}
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
			m_EntitySystemIds.emplace_back();
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

	void EntityManager::SetParent(EntityId entity, EntityId parent)
	{
		EntityId prev_parent = m_EntityParent[entity];

		// If already is parent
		if (prev_parent == parent) return;

		// If entity didn't have a parent
		if (prev_parent == entity) {
			size_t p_size = m_ParentEntitiesAlive.size();

			for (size_t i = 0; i < p_size; i++) {
				if (m_ParentEntitiesAlive[i] == entity) {
					m_ParentEntitiesAlive.erase(m_ParentEntitiesAlive.begin() + i);
					break;
				}
			}
		}
		// If entity had a parent
		else {
			std::vector<EntityId>& prev_parent_children = m_EntityChildren[prev_parent];
			size_t size = prev_parent_children.size();

			for (size_t i = 0; i < size; i++) {
				if (prev_parent_children[i] == entity) {
					prev_parent_children.erase(prev_parent_children.begin() + i);
					break;
				}
			}
		}

		m_EntityParent[entity] = parent;

		if (entity == parent) {
			m_ParentEntitiesAlive.push_back(entity);
		}
		else {
			m_EntityChildren[parent].push_back(entity);
		}
	}

	void EntityManager::DestroyEntity(EntityId entity)
	{
		m_EntitiesToDestroy.push_back(entity);
	}

	void EntityManager::ReserveEntities(size_t amount)
	{
		m_EntityComponents.reserve(amount);
		m_EntitySystemIds.reserve(amount);
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
		}

		m_ComponentsReserved[cid] = m_ComponentsSize[cid] + amount;
	}

	byte* EntityManager::AddComponent(EntityId entityId, ComponentHash hash, byte* value) {
		const Type* ctype = Application::Get().getCoreTypeH(hash);

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

	void EntityManager::RemoveComponent(EntityId entity, ComponentHash hash)
	{
		std::map<ComponentId, size_t>& c_map = m_EntityComponents[entity];

		std::map<ComponentId, size_t>::iterator c_it;

		for (c_it = c_map.begin(); c_it != c_map.end(); c_it++) {
			ComponentId c_id = c_it->first;
			const Type* c_type = m_ComponentTypes[c_id];
			
			if (c_type->hash == hash) {
				m_ComponentsRemoved[c_it->first].push_back(c_it->second);

				c_map.erase(c_it);

				break;
			}
		}
	}

	void EntityManager::RemoveComponentById(EntityId entity, ComponentId componentId)
	{
		std::map<ComponentId, size_t>& c_map = m_EntityComponents[entity];
		std::map<ComponentId, size_t>::iterator c_it = c_map.find(componentId);

		if (c_it != c_map.end()) {
			m_ComponentsRemoved[c_it->first].push_back(c_it->second);

			c_map.erase(c_it);
		}
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

			if (m_ComponentTypes.size() >= component_id) {
				m_ComponentTypes.resize(component_id + 1, NULL);
			}
			
			m_ComponentTypes[component_id] = type;
		}
		else {
			component_id = cid->second.cid;
		}

		return component_id;
	}

	ComponentId EntityManager::GetComponentId(ComponentHash hash) {
		const Type* ctype = Application::Get().GetComponentTypeH(hash);

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

	bool EntityManager::HasSystem(EntityId eid, SystemHash system_hash)
	{
		size_t size = m_EntitySystemIds[eid].size();

		for (size_t i = 0; i < size; i++) {
			if (m_EntitySystemIds[eid][i] == system_hash)
				return true;
		}

		return false;
	}

	size_t EntityManager::getSystemIndex(EntityId entityId, SystemHash system_hash)
	{
		size_t index = INVALID_INDEX;

		size_t size = m_EntitySystemIds[entityId].size();

		for (size_t i = 0; i < size; i++) {
			if (m_EntitySystemIds[entityId][i] == system_hash)
			{
				index = i;
				break;
			}
		}

		return index;
	}

	void EntityManager::ApplySystem(EntityId eid, SystemHash system_hash)
	{
		if (HasSystem(eid, system_hash)) return;

		const Type* stype = Application::Get().GetSystemTypeH(system_hash);

		if (stype) {
			System* system = (System*)stype->New();
			system->SetEntity(eid);
			system->Awake();

			m_EntitySystems[eid].push_back(system);
			m_EntitySystemIds[eid].push_back(system_hash);
		}
		else {
			WI_CORE_ERROR("System hash not found [{}]", system_hash);
		}
	}

	void EntityManager::ApplySystem(EntityId eid, const Type* system_type)
	{
		ApplySystem(eid, system_type->hash);
	}
}