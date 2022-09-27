#include "wipch.h"

#include "XMLDocument.h"

namespace Wiwa {
	XMLDocument::XMLDocument()
	{

	}

	XMLDocument::XMLDocument(const char* path)
	{
		Load(path);
	}

	void XMLDocument::Load(const char* path)
	{
		pugi::xml_parse_result result = m_Doc.load_file(path);

		if (result != NULL) {
			m_Node = m_Doc;
		}
	}

	void XMLDocument::Save(const char* path)
	{
		m_Doc.save_file(path);
	}
}