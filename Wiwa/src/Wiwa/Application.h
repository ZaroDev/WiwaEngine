#pragma once

#pragma warning(disable : 4251)

#include "Core.h"
#include "Window.h"
#include "Wiwa/LayerStack.h"
#include "Wiwa/Events/Event.h"
#include "Wiwa/Events/ApplicationEvent.h"
#include "Wiwa/ImGui/ImGuiLayer.h"

#include <Wiwa/utilities/math/Vector2i.h>
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/Reflection.h>

namespace Wiwa {
	
	class EntityManager;
	class Renderer2D;
	class Renderer3D;

	struct SysInfo
	{
		SysInfo() = default;

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
		inline Renderer2D& GetRenderer2D(){ return *m_Renderer2D; }
		inline Renderer3D& GetRenderer3D() { return *m_Renderer3D; }
		inline EntityManager& GetEntityManager() { return *m_EntityManager; }
		inline ImGuiLayer& GetImGuiLayer() { return *m_ImGuiLayer; }

		inline float GetDeltaTime() { return m_Time; }
		inline ImGuiContext* GetImGuiContext() { return m_ImGuiLayer->GetContext(); }
		
		inline SysInfo& GetSystemInfo() { return m_SysInfo; }
		inline const char* GetVersion() { return m_Version; }

		inline void SetTargetResolution(int width, int height) { m_TargetResolution = { width, height }; }
		inline Size2i& GetTargetResolution() { return m_TargetResolution; }

		// Reflection
		size_t getTypeCount() const;
		const Type* getType(size_t index) const;
		const Type* getTypeH(size_t hash) const;

		void OpenDir(const char* url);
		void Quit();

	private:
		Size2i m_TargetResolution;

		bool OnWindowClose(WindowCloseEvent& e);

		const char* m_Version = "0.1";

		float m_Time = 0.0f;

		Renderer2D* m_Renderer2D;
		Renderer3D* m_Renderer3D;

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