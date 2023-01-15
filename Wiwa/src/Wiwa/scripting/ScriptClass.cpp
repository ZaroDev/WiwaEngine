#include <wipch.h>
#include "ScriptClass.h"

#include "ScriptEngine.h"
#include "ScriptUtils.h"

#include <mono/metadata/object.h>

namespace Wiwa {
	ScriptClass::ScriptClass(MonoAssembly* assembly, const std::string& classNamespace, const std::string& className)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = Utils::GetClassInAssembly(assembly, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	MonoClassField* ScriptClass::GetField(const std::string& name)
	{
		return mono_class_get_field_from_name(m_MonoClass, name.c_str());
	}

	void ScriptClass::SetFieldValue(MonoObject* instance, MonoClassField* field, void* value)
	{
		mono_field_set_value(instance, field, value);
	}
}