#include "Panel.h"

Panel::Panel(const char* name, EditorLayer* instance_) : name(name), instance(instance_)
{}

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

