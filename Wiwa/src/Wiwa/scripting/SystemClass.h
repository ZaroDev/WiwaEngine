#pragma once

#include <string>
extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoArray MonoArray;
}

namespace Wiwa {
	class SystemClass
	{
	public:
		SystemClass() = default;
		SystemClass(const std::string& classNamespace, const std::string& className);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
		MonoClass* m_MonoClass = nullptr;
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
	};
}