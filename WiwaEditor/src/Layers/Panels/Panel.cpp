#include "Panel.h"

Panel::Panel(const char* name, const char* icon, EditorLayer* instance_) 
	: name(name),icon(icon), instance(instance_)
{
	iconName = std::string(icon).append(name).c_str();
}

Panel::~Panel()
{}

void Panel::SetActive(bool enabled)
{
	active = enabled;
}

void Panel::SwitchActive()
{
	active = !active;
}

bool Panel::IsActive() const
{
	return active;
}


