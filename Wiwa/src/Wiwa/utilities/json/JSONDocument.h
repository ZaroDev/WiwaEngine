#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/Core.h>
#include "../vendor/rapidjson/rapidjson.h"
#include "JSONValue.h"
#include <string>

namespace Wiwa {
	class WI_API JSONDocument {
	private:
		rapidjson::Document m_Document;
	public:
		JSONDocument();
		~JSONDocument();

		void Parse(const char* string);
		const char* getString();

		JSONValue operator[](const char* key);
	};
}