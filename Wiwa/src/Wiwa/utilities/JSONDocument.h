#pragma once
#pragma warning(disable : 4251)

#include "Empathy/Core.h"
#include "Vendor/rapidjson/document.h"
#include "JSONValue.h"
#include <string>

namespace EM {
	class EM_API JSONDocument {
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