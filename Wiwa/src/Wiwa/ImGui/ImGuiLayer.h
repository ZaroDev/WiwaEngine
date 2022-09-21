#pragma once

#include "Wiwa/Layer.h"

#include "Wiwa/Events/Event.h"
#include "Wiwa/Events/KeyEvent.h"
#include "Wiwa/Events/MouseEvent.h"
#include "Wiwa/Events/ApplicationEvent.h"

namespace Wiwa {
	class WI_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();


		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMoveEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

		void SetDarkTheme();
	private:
		float m_Time = 0.0f;
		bool m_ShowDemo = false;
	};
}