#include "wipch.h"

#include "XMLNode.h"

namespace Wiwa {
	XMLNode::XMLNode()
	{
	}

	XMLNode::XMLNode(const XMLNode& node)
	{
		m_Node = node.m_Node;
	}

	XMLNode XMLNode::first_child() const
	{
		XMLNode node;

		node.m_Node = m_Node.first_child();

		pugi::xml_attribute attr;

		return node;
	}

	XMLNode XMLNode::child(const char* name)
	{
		XMLNode node;

		node.m_Node = m_Node.child(name);

		return node;
	}

	XMLNode XMLNode::append_child(const char* name)
	{
		XMLNode node;

		node.m_Node = m_Node.append_child(name);

		return node;
	}

	XMLNode XMLNode::next_sibling()
	{
		XMLNode node;

		node.m_Node = m_Node.next_sibling();

		return node;
	}

	XMLNode XMLNode::next_sibling(const char* name)
	{
		XMLNode node;

		node.m_Node = m_Node.next_sibling(name);

		return node;
	}

	XMLAttribute XMLNode::append_attribute(const char* name)
	{
		XMLAttribute attr;

		attr.m_Attr = m_Node.append_attribute(name);

		return attr;
	}

	XMLAttribute XMLNode::attribute(const char* name)
	{
		XMLAttribute attr;

		attr.m_Attr = m_Node.attribute(name);

		return attr;
	}

	XMLNode::operator bool()
	{
		return m_Node;
	}
}