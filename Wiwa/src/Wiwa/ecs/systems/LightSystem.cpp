#include <wipch.h>
#include "LightSystem.h"
#include <Wiwa\utilities\render\LightManager.h>
#include <Wiwa\scene\SceneManager.h>
#include <Wiwa\scene\Scene.h>
#include <Wiwa\utilities\render\LightManager.h>

Wiwa::LightSystem::LightSystem()
{
	SceneManager::getActiveScene()->GetLightManager().AddLight(m_EntityId);
}

Wiwa::LightSystem::~LightSystem()
{
	SceneManager::getActiveScene()->GetLightManager().RemoveLight(m_EntityId);
}

