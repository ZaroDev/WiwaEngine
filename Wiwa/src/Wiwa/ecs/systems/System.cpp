#include <wipch.h>
#include "System.h"

namespace Wiwa {
	//-------- CONSTRUCTOR --------
	System::System()
	{
	}

	//-------- DESTRUCTOR --------
	System::~System()
	{
	}

	//-------- Entity Functions --------
	size_t System::getEntityPos(EntityId eid)
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

	void System::AddEntity(EntityId entity)
	{
		size_t index = getEntityPos(entity);

		if (index == m_RegisteredEntities.size()) {
			m_RegisteredEntities.emplace_back(entity);
			OnEntityAdded(entity);
		}
	}

	void System::RemoveEntity(EntityId eid)
	{
		size_t index = getEntityPos(eid);

		if (index < m_RegisteredEntities.size()) {
			m_RegisteredEntities.erase(m_RegisteredEntities.begin() + index);
			OnEntityRemoved(eid);
		}
	}

	void System::Reserve(size_t amount)
	{
		m_RegisteredEntities.reserve(amount);
	}

	//-------- Sub-system functions --------
	void System::Awake()
	{
		size_t size = m_RegisteredEntities.size();

		for (size_t i = 0; i < size; i++) {
			OnAwake(m_RegisteredEntities[i]);
		}
	}

	void System::Init()
	{
		size_t size = m_RegisteredEntities.size();

		for (size_t i = 0; i < size; i++) {
			OnInit(m_RegisteredEntities[i]);
		}
	}

	void System::Update()
	{
		size_t size = m_RegisteredEntities.size();

		for (size_t i = 0; i < size; i++) {
			OnUpdate(m_RegisteredEntities[i]);
		}
	}
}