#include "../../headers/utilities/JSONDocument.h"
#include "../../../Vendor/rapidjson/stringbuffer.h"
#include "../../../Vendor/rapidjson/writer.h"
#include <iostream>

namespace EM {
	EM::JSONDocument::JSONDocument()
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
