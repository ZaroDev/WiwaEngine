#include "wipch.h"

#include "JSONValue.h"

namespace Wiwa {
	JSONValue::JSONValue(rapidjson::Value* value)
	{
		m_Value = value;
	}
	JSONValue::~JSONValue()
	{
		
	}
}
