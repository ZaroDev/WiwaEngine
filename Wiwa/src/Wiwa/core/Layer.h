#pragma once

#include <Wiwa/core/Core.h>
#include "Wiwa/Events/Event.h"

namespace Wiwa {
	class WI_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach(){}
		virtual void OnDetach(){}
		virtual void OnUpdate(){}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}

