#include <wipch.h>
#include "System.h"

namespace Wiwa {
	//-------- CONSTRUCTOR --------
	System::System() : m_EntityId(EntityManager::INVALID_INDEX)
	{
	}

	//-------- DESTRUCTOR --------
	System::~System()
	{
	}

	//-------- Entity Functions --------

	//-------- Sub-system functions --------
	void System::Awake()
	{
		OnAwake();
	}

	void System::Init()
	{
		OnInit();
	}

	void System::Update()
	{
		OnUpdate();
	}

	void System::Destroy()
	{
		OnDestroy();
	}
}