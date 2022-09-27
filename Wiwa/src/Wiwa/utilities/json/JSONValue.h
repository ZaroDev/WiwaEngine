#pragma once

#include <Wiwa/Core.h>
#include "../vendor/rapidjson/document.h"

namespace Wiwa {
	class WI_API JSONValue {
	private:
		rapidjson::Value* m_Value;
	public:
		JSONValue(rapidjson::Value* value);
		~JSONValue();

		template<class T> void Set(T data);
	};

	template<class T>
	inline void JSONValue::Set(T data)
	{
		m_Value->Set<T>(data);
	}
}