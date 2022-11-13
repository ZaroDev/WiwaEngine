#pragma once

#include <Wiwa/core/Core.h>
#include "../vendor/pugixml/src/pugixml.hpp"

namespace Wiwa {
	class WI_API XMLAttribute {
		friend class XMLNode;
	private:
		pugi::xml_attribute m_Attr;
	public:
		XMLAttribute();
		XMLAttribute(const XMLAttribute& attr);

		const char* as_string() const;

		bool set_value(const char* value);


		XMLAttribute& operator=(const char* value);
		XMLAttribute& operator=(int value);
	};
}