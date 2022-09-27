#include "wipch.h"
#include "Application.h"

#include <Wiwa/ecs/EntityManager.h>

#include "Wiwa/Log.h"

#include <glew.h>
#include <GLFW/glfw3.h>

#include "Input.h"

#include <shellapi.h>
#include <Windows.h>

namespace Wiwa {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		WI_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		int min, major, rev;

		glfwGetVersion(&min, &major, &rev);
		sprintf_s(m_SysInfo.glfwVer, 32, "%i.%i.%i", min, major, rev);

		SYSTEM_INFO info;
		::GetSystemInfo(&info);


		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_SysInfo.gpu = glGetString(GL_VENDOR);
		m_SysInfo.gpuBrand = glGetStringi(GL_RENDERER, 0);
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_EntityManager = new EntityManager();
		PushLayer(m_EntityManager);
	}

	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Time = (float)glfwGetTime();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			{
				//TODO: Optick On ImGuiRender call

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
	void Application::RequestBrowser(const char* url)
	{
		ShellExecuteA(0, "open", url, NULL, NULL, SW_SHOWNORMAL);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));



		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}