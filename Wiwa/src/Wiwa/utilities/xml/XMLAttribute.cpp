#include "wipch.h"

#include "XMLAttribute.h"

namespace Wiwa {
	XMLAttribute::XMLAttribute()
	{
	}

	XMLAttribute::XMLAttribute(const XMLAttribute& attr)
	{
		m_Attr = attr.m_Attr;
	}

	const char* XMLAttribute::as_string() const
	{
		return m_Attr.as_string();
	}

	bool XMLAttribute::set_value(const char* value)
	{
		return m_Attr.set_value(value);
	}

	XMLAttribute& XMLAttribute::operator=(const char* value)
	{
		m_Attr = value;

		return *this;
	}

	XMLAttribute& XMLAttribute::operator=(int value)
	{
		m_Attr = value;

		return *this;
	}
}