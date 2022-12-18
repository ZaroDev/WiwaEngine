#pragma once

#include <string>

#include "ScriptClass.h"

#include <Wiwa/ecs/systems/System.h>

namespace Wiwa {
	class SystemScriptClass : public System
	{
	public:
		SystemScriptClass() = default;
		~SystemScriptClass() = default;

		SystemScriptClass(const std::string& classNamespace, const std::string& className);

		void OnAwake(EntityId eid) override;

		void OnInit(EntityId eid) override;

		void OnUpdate(EntityId eid) override;

	private:
		ScriptClass scriptClass;
		MonoObject* m_SystemObject;

		MonoMethod* m_AwakeMethod;
		MonoMethod* m_InitMethod;
		MonoMethod* m_UpdateMethod;
	};
}