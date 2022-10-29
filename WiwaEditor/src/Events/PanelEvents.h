#pragma once
#include <Wiwa/Events/Event.h>

typedef size_t ResourceId;

class ResourceChange : public Wiwa::Event
{
public:
	ResourceChange(ResourceId id) : m_Id(id){}

	ResourceId GetResourceId() { return m_Id; }

	EVENT_CLASS_CATEGORY(Wiwa::EventCategoryEditor)
protected:
	ResourceId m_Id;
};
class MaterialChangeEvent : public ResourceChange
{
public:
	MaterialChangeEvent(ResourceId id) : ResourceChange(id){}
	EVENT_CLASS_TYPE(OnMaterialChange)
};

class EntityChangeEvent : public Wiwa::Event
{
public:
	EntityChangeEvent(size_t id) : m_Id(id) {}

	size_t GetResourceId() { return m_Id; }

	EVENT_CLASS_TYPE(OnEntityChange)
		EVENT_CLASS_CATEGORY(Wiwa::EventCategoryEditor)
private:
	size_t m_Id;
};

