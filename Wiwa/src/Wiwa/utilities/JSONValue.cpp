#include "../../headers/utilities/JSONValue.h"

namespace EM {
	JSONValue::JSONValue(rapidjson::Value* value)
	{
		m_Value = value;
	}
	JSONValue::~JSONValue()
	{
		
	}
}
