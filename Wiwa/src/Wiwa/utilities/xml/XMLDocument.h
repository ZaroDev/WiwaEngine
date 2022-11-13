#pragma once

#include <Wiwa/core/Core.h>
#include "XMLNode.h"
#include "../vendor/pugixml/src/pugixml.hpp"

namespace Wiwa {
	class WI_API XMLDocument : public XMLNode {
	private:
		pugi::xml_document m_Doc;
	public:
		XMLDocument();
		XMLDocument(const char* path);
		
		void Load(const char* path);
		void Save(const char* path);
	};
}