#include <wipch.h>
#include "LightManager.h"

Wiwa::LightManager::LightManager()
{
}

Wiwa::LightManager::~LightManager()
{
}

void Wiwa::LightManager::AddLight(size_t light)
{
	m_Lights.push_back(light);
}

void Wiwa::LightManager::RemoveLight(size_t light)
{
	for (size_t i = 0; i < m_Lights.size(); i++)
	{
		if (m_Lights[i] == light)
		{
			m_Lights.erase(m_Lights.begin() + i);
			break;
		}
	}
}
