#pragma once

#include "Wiwa/Layer.h"

#include "Wiwa/Events/Event.h"
#include "Wiwa/Events/KeyEvent.h"
#include "Wiwa/Events/MouseEvent.h"
#include "Wiwa/Events/ApplicationEvent.h"

struct ImGuiContext;

namespace Wiwa {
	class WI_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void Begin();
		void End();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event) override;
		
		inline ImGuiContext* GetContext() { return m_Ctx; }
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
		ImGuiContext* m_Ctx = nullptr;
		float m_Time = 0.0f;
		bool m_ShowDemo = false;
	};
}