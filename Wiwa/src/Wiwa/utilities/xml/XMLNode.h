#pragma once

#include "Empathy/Core.h"
#include "Vendor/pugixml/src/pugixml.hpp"

#include "XMLAttribute.h"

namespace EM {
	class EM_API XMLNode {
	private:
		
	protected:
		pugi::xml_node m_Node;
	public:
		XMLNode();
		XMLNode(const XMLNode& node);

		XMLNode first_child() const;
		XMLNode child(const char* name);

		XMLNode append_child(const char* name);

		XMLNode next_sibling();
		XMLNode next_sibling(const char* name);

		XMLAttribute append_attribute(const char* name);
		XMLAttribute attribute(const char* name);

		operator bool();
	};
}