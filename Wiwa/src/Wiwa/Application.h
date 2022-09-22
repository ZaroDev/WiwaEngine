#pragma once

#include "Core.h"
#include "Window.h"
#include "Wiwa/LayerStack.h"
#include "Wiwa/Events/Event.h"
#include "Wiwa/Events/ApplicationEvent.h"

namespace Wiwa {
	class EntityManager;

	class WI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		inline EntityManager* GetEntityManager() { return m_EntityManager; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		EntityManager* m_EntityManager;
	private:
		static Application* s_Instance;
	};

	//To be defined in a client
	Application* CreateApplication();
}

