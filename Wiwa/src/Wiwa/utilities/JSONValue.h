#pragma once

#include "Empathy/Core.h"
#include "../../../Vendor/rapidjson/document.h"

namespace EM {
	class EM_API JSONValue {
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