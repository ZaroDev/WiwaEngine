#include "Empathy/headers/utilities/xml/XMLAttribute.h"

EM::XMLAttribute::XMLAttribute()
{
}

EM::XMLAttribute::XMLAttribute(const XMLAttribute& attr)
{
	m_Attr = attr.m_Attr;
}

const char* EM::XMLAttribute::as_string() const
{
	return m_Attr.as_string();
}

bool EM::XMLAttribute::set_value(const char* value)
{
	return m_Attr.set_value(value);
}

EM::XMLAttribute& EM::XMLAttribute::operator=(const char* value)
{
	m_Attr = value;

	return *this;
}

EM::XMLAttribute& EM::XMLAttribute::operator=(int value)
{
	m_Attr = value;

	return *this;
}