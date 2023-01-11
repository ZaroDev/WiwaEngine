#include <wipch.h>

#include "SystemScriptClass.h"
#include "ScriptUtils.h"
#include "ScriptEngine.h"

#include <mono/metadata/object.h>

namespace Wiwa {
	SystemScriptClass::SystemScriptClass(const std::string& classNamespace, const std::string& className) :
		scriptClass(classNamespace, className)
	{
		m_SystemObject = scriptClass.Instantiate();

		m_AwakeMethod = scriptClass.GetMethod("Awake", 1);
		m_InitMethod = scriptClass.GetMethod("Init", 1);
		m_UpdateMethod = scriptClass.GetMethod("Update", 1);
	}

	void SystemScriptClass::OnAwake(EntityId eid)
	{
		if (!m_AwakeMethod) return;

		void* params[]{
			&eid
		};
		scriptClass.InvokeMethod(m_SystemObject, m_AwakeMethod, params);
	}

	void SystemScriptClass::OnInit(EntityId eid)
	{
		if (!m_InitMethod) return;

		void* params[]{
			&eid
		};
		scriptClass.InvokeMethod(m_SystemObject, m_InitMethod, params);
	}

	void SystemScriptClass::OnUpdate(EntityId eid)
	{
		if (!m_UpdateMethod) return;

		void* params[]{
			&eid
		};
		scriptClass.InvokeMethod(m_SystemObject, m_UpdateMethod, params);
	}
}