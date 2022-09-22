#pragma once

#include "Empathy/Core.h"
#include "XMLNode.h"
#include "Vendor/pugixml/src/pugixml.hpp"

namespace EM {
	class EM_API XMLDocument : public XMLNode {
	private:
		pugi::xml_document m_Doc;
	public:
		XMLDocument();
		XMLDocument(const char* path);
		
		void Load(const char* path);
		void Save(const char* path);
	};
}