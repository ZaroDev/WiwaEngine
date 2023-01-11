#include <wipch.h>

#include "SystemClass.h"
#include "ScriptUtils.h"
#include "ScriptEngine.h"

#include <mono/metadata/object.h>

namespace Wiwa {
	SystemClass::SystemClass(const std::string& classNamespace, const std::string& className)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = Utils::GetClassInAssembly(ScriptEngine::s_Data->SystemAssembly, classNamespace.c_str(), className.c_str());
	}
	MonoObject* SystemClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}
	MonoMethod* SystemClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}
	MonoObject* SystemClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}
}