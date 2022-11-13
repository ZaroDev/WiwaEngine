#pragma once

#include <Wiwa/core/Core.h>
#include "../vendor/rapidjson/document.h"

namespace Wiwa {
	class WI_API JSONValue {
	private:
		rapidjson::Value* m_Value;
	public:
		JSONValue(rapidjson::Value* value);
		~JSONValue();

		template<class T> void set(T data);
		template<class T> void operator=(T data);

		template<class T> T get();

		template<class T> operator T();
	};

	template<class T>
	inline void JSONValue::set(T data)
	{
		m_Value->Set<T>(data);
	}

	template<class T>
	inline T JSONValue::get()
	{
		return m_Value->Get<T>();
	}
	template<>
	inline const char* JSONValue::get()
	{
		return m_Value->GetString();
	}

	template<class T>
	inline void JSONValue::operator=(T data)
	{
		set<T>(data);
	}

	template<class T>
	inline JSONValue::operator T()
	{
		return get<T>();
	}

}