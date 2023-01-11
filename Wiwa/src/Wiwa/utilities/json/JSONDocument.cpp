#include "wipch.h"
#include "JSONDocument.h"

#include "../vendor/rapidjson/writer.h"
#include "../vendor/rapidjson/filereadstream.h"
#include "../vendor/rapidjson/filewritestream.h"

namespace Wiwa {

	JSONDocument::JSONDocument(const char* path)
	{
		if (!load_file(path)) {
			WI_WARN("Couldn't load file \"{0}\"", path);
		}
	}

	JSONDocument::JSONDocument()
	{
		m_Document.SetObject();
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
		m_Buffer.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(m_Buffer);

		m_Document.Accept(writer);

		return m_Buffer.GetString();
	}

	JSONValue JSONDocument::AddMemberObject(const char* mem)
	{
		rapidjson::Value key(mem, m_Document.GetAllocator());
		rapidjson::Value v(rapidjson::kObjectType);

		m_Document.AddMember(key, v, m_Document.GetAllocator());

		rapidjson::Value& jval = m_Document[mem];

		return JSONValue(&jval, &m_Document.GetAllocator());
	}

	bool JSONDocument::HasMember(const char* mem)
	{
		return m_Document.HasMember(mem);
	}

	bool JSONDocument::load_file(const char* path)
	{
		FILE* fp = fopen(path, "rb");

		if (!fp)
			return false;

		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		m_Document.ParseStream(is);

		fclose(fp);
		return true;
	}

	bool JSONDocument::save_file(const char* path)
	{
		FILE* fp = fopen(path, "wb");

		if (!fp)
			return false;

		char writeBuffer[65536];
		rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

		rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
		m_Document.Accept(writer);

		fclose(fp);

		return true;
	}

	JSONValue JSONDocument::operator[](const char * key)
	{
		return JSONValue(&m_Document[key], &m_Document.GetAllocator());
	}
}
