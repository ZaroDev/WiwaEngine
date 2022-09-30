#pragma once

#include "Core.h"
#include "Window.h"
#include "Wiwa/LayerStack.h"
#include "Wiwa/Events/Event.h"
#include "Wiwa/Events/ApplicationEvent.h"
#include "Wiwa/ImGui/ImGuiLayer.h"


namespace Wiwa {
	
	class EntityManager;

	struct SysInfo
	{
		char glfwVer[32];
		uint32_t cache;
		uint32_t numCores;
		float ram;
		const unsigned char* gpu;
		const unsigned char* gpuBrand;
		float gpuVRAM;
		float gpuVRAMUsage;
		float gpuVRAMAV;
		float gpuVRAMReserve;
	};

	class WI_API Application
	{	
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void SetHwInfo();

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		inline float GetDeltaTime() { return m_Time; }
		inline ImGuiContext* GetImGuiContext() { return m_ImGuiLayer->GetContext(); }
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline EntityManager* GetEntityManager() { return m_EntityManager; }
		inline SysInfo& GetSystemInfo() { return m_SysInfo; }
		inline const char* GetVersion() { return m_Version; }


		void OpenDir(const char* url);
		void Quit();

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		const char* m_Version = "0.1";

		float m_Time = 0.0f;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		SysInfo m_SysInfo;
		ImGuiLayer* m_ImGuiLayer;
		EntityManager* m_EntityManager;
	private:
		static Application* s_Instance;
	};

	//To be defined in a client
	Application* CreateApplication();
}

