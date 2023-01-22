#pragma once

#include <string>

#include "ScriptClass.h"

#include <Wiwa/ecs/systems/System.h>

namespace Wiwa {
	class SystemScriptClass : public System
	{
	public:
		SystemScriptClass() = default;
		~SystemScriptClass();

		SystemScriptClass(MonoAssembly* assembly,const std::string& classNamespace, const std::string& className);

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnEntitySet() override;
	private:
		ScriptClass scriptClass;

		MonoObject* m_SystemObject;

		MonoMethod* m_AwakeMethod;
		MonoMethod* m_InitMethod;
		MonoMethod* m_UpdateMethod;

		MonoClassField* m_EntityIdField;
	};
}