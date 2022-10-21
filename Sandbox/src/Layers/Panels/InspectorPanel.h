#pragma once

#include "Panel.h"

#include <Wiwa/Reflection.h>
#include <Wiwa/utilities/containers/Array.h>

#include <Wiwa/utilities/math/Vector3f.h>

namespace Wiwa {
	class EntityManager;
};

class InspectorPanel : public Panel
{
private:
	void ClearComponentName(std::string& cname);

	void DrawComponent(size_t componentId);
	void DrawField(unsigned char* data, const Field& field);

	void DrawVec3Control(const char* label, unsigned char* data, const Field field , float resetValue = 0.0f, float columnWidth = 100.0f);
	void DrawVec2Control(const char* label, unsigned char* data, const Field field , float resetValue = 0.0f, float columnWidth = 100.0f);
	void DrawInt2Control(const char* label, unsigned char* data, const Field field , float resetValue = 0.0f, float columnWidth = 100.0f);
	void DrawRect2Control(const char* label, unsigned char* data, const Field field , float resetValue = 0.0f, float columnWidth = 100.0f);

	Wiwa::EntityManager* m_EntityManager;
public:
	InspectorPanel();
	virtual ~InspectorPanel();

	void Draw() override;
	void Update() override;

	void OnEvent(Wiwa::Event& e) override;
public:
	static inline void SetEntity(uint32_t entityID) { m_CurrentID = entityID; m_EntitySet = true; };
	static inline bool GetCurrentEntity(uint32_t& entityID) 
	{
		entityID = m_CurrentID;
		return m_EntitySet; 
	}
	static uint32_t m_CurrentID;
	static bool m_EntitySet;
};