#pragma once
#include "Event.h"


namespace Wiwa {
	class WI_API MeshChange : public Event
	{
	public:
		MeshChange(uint32_t modelId, uint32_t matId)
			: m_ModelId(modelId), m_MatId(matId) {}
		EVENT_CLASS_TYPE(OnMeshChange)
		EVENT_CLASS_CATEGORY(EventCategoryEditorEvent)
	private:
		uint32_t m_ModelId, m_MatId;
	};

	class WI_API MaterialChange : public Event
	{
	public:
		MaterialChange(uint32_t matId)
			: m_MatId(matId) {}

		inline uint32_t GetMatId() { return m_MatId; }

		EVENT_CLASS_TYPE(OnMaterialChange)
		EVENT_CLASS_CATEGORY(EventCategoryEditorEvent)
	private:
		uint32_t m_MatId;
	};
}