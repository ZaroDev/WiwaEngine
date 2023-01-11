#include "wipch.h"

#include "JSONValue.h"

namespace Wiwa {
	JSONValue::JSONValue(rapidjson::Value* value, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>* allocator)
	{
		m_Value = value;
		m_Allocator = allocator;
	}

	JSONValue::~JSONValue()
	{
		
	}

	JSONValue JSONValue::AddMemberObject(const char* mem)
	{
		rapidjson::Value key(mem, *m_Allocator);
		rapidjson::Value v(rapidjson::kObjectType);

		m_Value->AddMember(key, v, *m_Allocator);

		rapidjson::Value& jval = m_Value->operator[](mem);

		return JSONValue(&jval, m_Allocator);
	}

	bool JSONValue::HasMember(const char* mem)
	{
		return m_Value->HasMember(mem);
	}

	JSONValue JSONValue::operator[](const char* key)
	{
		return JSONValue(&m_Value->operator[](key), m_Allocator);
	}
}
