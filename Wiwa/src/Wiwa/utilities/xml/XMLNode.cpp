#include "Empathy/headers/utilities/xml/XMLNode.h"

EM::XMLNode::XMLNode()
{
}

EM::XMLNode::XMLNode(const XMLNode& node)
{
	m_Node = node.m_Node;
}

EM::XMLNode EM::XMLNode::first_child() const
{
	EM::XMLNode node;

	node.m_Node = m_Node.first_child();

	pugi::xml_attribute attr;

	return node;
}

EM::XMLNode EM::XMLNode::child(const char* name)
{
	EM::XMLNode node;

	node.m_Node = m_Node.child(name);

	return node;
}

EM::XMLNode EM::XMLNode::append_child(const char* name)
{
	EM::XMLNode node;

	node.m_Node = m_Node.append_child(name);

	return node;
}

EM::XMLNode EM::XMLNode::next_sibling()
{
	EM::XMLNode node;

	node.m_Node = m_Node.next_sibling();

	return node;
}

EM::XMLNode EM::XMLNode::next_sibling(const char* name)
{
	EM::XMLNode node;

	node.m_Node = m_Node.next_sibling(name);

	return node;
}

EM::XMLAttribute EM::XMLNode::append_attribute(const char* name)
{
	EM::XMLAttribute attr;

	attr.m_Attr = m_Node.append_attribute(name);

	return attr;
}

EM::XMLAttribute EM::XMLNode::attribute(const char* name)
{
	EM::XMLAttribute attr;

	attr.m_Attr = m_Node.attribute(name);

	return attr;
}

EM::XMLNode::operator bool()
{
	return m_Node;
}