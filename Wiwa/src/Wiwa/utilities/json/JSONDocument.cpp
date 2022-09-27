#include "wipch.h"

#include "JSONDocument.h"
#include "../vendor/rapidjson/stringbuffer.h"
#include "../vendor/rapidjson/writer.h"

namespace Wiwa {
	JSONDocument::JSONDocument()
	{
	}

	JSONDocument::~JSONDocument()
	{
	}

	void JSONDocument::Parse(const char * string)
	{
		m_Document.Parse(string);
	}

	const char* JSONDocument::getString()
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

		m_Document.Accept(writer);

		return buffer.GetString();
	}

	JSONValue JSONDocument::operator[](const char * key)
	{
		return JSONValue(&m_Document[key]);
	}
}
