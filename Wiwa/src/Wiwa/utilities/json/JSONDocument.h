#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/core/Core.h>

#include "../vendor/rapidjson/rapidjson.h"
#include "../vendor/rapidjson/stringbuffer.h"

#include "JSONValue.h"
#include <string>

namespace Wiwa {
	class WI_API JSONDocument {
	private:
		rapidjson::Document m_Document;
		rapidjson::StringBuffer m_Buffer;
	public:
		JSONDocument(const char* path);
		JSONDocument();
		~JSONDocument();

		void Parse(const char* string);
		const char* getString();

		template<class T>
		JSONValue AddMember(const char* mem, T value);

		JSONValue AddMemberObject(const char* mem);

		bool HasMember(const char* mem);

		bool load_file(const char* path);
		bool save_file(const char* path);

		JSONValue operator[](const char* key);
	};

	template<class T>
	inline JSONValue JSONDocument::AddMember(const char* mem, T value)
	{
		rapidjson::Value key(mem, m_Document.GetAllocator());
		rapidjson::Value v(value);

		rapidjson::Value& jval = m_Document.AddMember(key, v, m_Document.GetAllocator());

		return JSONValue(&jval, &m_Document.GetAllocator());
	}

	template<>
	inline JSONValue JSONDocument::AddMember(const char* mem, int value)
	{
		rapidjson::Value key(mem, m_Document.GetAllocator());
		rapidjson::Value v(value);

		rapidjson::Value& jval = m_Document.AddMember(key, v, m_Document.GetAllocator());

		return JSONValue(&jval, &m_Document.GetAllocator());
	}

	template<>
	inline JSONValue JSONDocument::AddMember<const char*>(const char* mem, const char* value) {
		rapidjson::Value key(mem, m_Document.GetAllocator());
		rapidjson::Value v;
		v.SetString(value, m_Document.GetAllocator());

		rapidjson::Value& jval = m_Document.AddMember(key, v, m_Document.GetAllocator());

		return JSONValue(&jval, &m_Document.GetAllocator());
	}
	template<>
	inline JSONValue JSONDocument::AddMember<char*>(const char* mem,char* value) {
		rapidjson::Value key(mem, m_Document.GetAllocator());
		rapidjson::Value v;
		v.SetString(value, m_Document.GetAllocator());

		rapidjson::Value& jval = m_Document.AddMember(key, v, m_Document.GetAllocator());

		return JSONValue(&jval, &m_Document.GetAllocator());
	}
}