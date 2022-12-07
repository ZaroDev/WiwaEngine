
#include "wipch.h"
#include "Application.h"

#include <Wiwa/ecs/EntityManager.h>

#include <Wiwa/utilities/Log.h>

#include <glew.h>
#include <GLFW/glfw3.h>

#include "Input.h"

#include <shellapi.h>
#include <Windows.h>

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#include "Renderer2D.h"
#include "Renderer3D.h"

#include <Wiwa/ecs/systems/SpriteRenderer.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>

#include <Wiwa/ecs/components/Transform3D.h>

#include <Wiwa/scene/SceneManager.h>

#include <Wiwa/utilities/json/JSONDocument.h>

#include <Wiwa/utilities/AllocationMetrics.h>

#include <Wiwa/scripting/ScriptEngine.h>
#include <Wiwa/core/Resources.h>

const size_t TYPE_COUNT = __COUNTER__;

namespace Wiwa {
	Application* Application::s_Instance = nullptr;

	Application::Application(int argc, char** argv)
	{
		WI_CORE_ASSERT(!s_Instance, "Application already exists!");

		REFLECTION_REGISTER();

		m_ArgC = argc;

		for (int i = 0; i < argc; i++) {
			m_Argv.push_back(argv[i]);
		}

		s_Instance = this;

		m_TargetResolution = { 1920, 1080 };

		Resources::Import<Model>("Models/BakerHouse.fbx");
		
		//JSONDocument project("config/project.json");
		//m_ProjectName = project["name"].get<const char*>();
		//m_ProjectVersion = project["version"].get<const char*>();
		//m_ProjectCompany = project["company"].get<const char*>();
		//m_ProjectTarget = (ProjectTarget)project["target"].get<int>();
		//project.save_file("config/project.json");

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps("Wiwa Engine: " + m_ProjectName)));
		m_Window->SetEventCallback({ &Application::OnEvent, this });

		int min, major, rev;
		glfwGetVersion(&min, &major, &rev);
		sprintf_s(m_SysInfo.glfwVer, 32, "%i.%i.%i", min, major, rev);

		SetHwInfo();

		/*m_Renderer2D = new Renderer2D();
		m_Renderer2D->Init();*/
		
		m_Renderer3D = new Renderer3D();
		m_Renderer3D->Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_RenderColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		ScriptEngine::Init();
		
	}

	void Application::SetHwInfo()
	{
		SYSTEM_INFO info;
		::GetSystemInfo(&info);

		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		GLint total_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_mem_kb);
		GLint cur_avail_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb);
		GLint cur_reserv_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &cur_reserv_mem_kb);

		m_SysInfo.numCores = info.dwNumberOfProcessors;
		m_SysInfo.ram = (float)(memInfo.ullTotalPhys >> 20);
		m_SysInfo.gpu = (const unsigned char*)glGetString(GL_VENDOR);
		m_SysInfo.gpuBrand = glGetString(GL_RENDERER);
		m_SysInfo.gpuVRAM = (float)(total_mem_kb >> 10);
		m_SysInfo.gpuVRAMAV = (float)(cur_avail_mem_kb >> 10);
		m_SysInfo.gpuVRAMUsage = (float)((total_mem_kb - cur_avail_mem_kb) >> 10);
		m_SysInfo.gpuVRAMReserve = (float)(cur_reserv_mem_kb >> 10);
	}

	Application::~Application()
	{
		SceneManager::CleanUp();
		ScriptEngine::ShutDown();
		
	}

	void Application::Run()
	{
		while (m_Running)
		{
			OPTICK_FRAME("Application Loop");
			glClearColor(m_RenderColor.r, m_RenderColor.g, m_RenderColor.b, m_RenderColor.a);
			glClear(GL_COLOR_BUFFER_BIT);

			//m_Renderer2D->Update();
			SceneManager::Update();
			
			m_Renderer3D->Update();

			// Update time
			Time::Update();
			
			// Update scene manager

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

	size_t Application::getCoreTypeCount() const
	{
		return TYPE_COUNT;
	}

	const Type* Application::getCoreType(size_t index) const
	{
		return GET_TYPES()->at(index);
	}

	const Type* Application::getCoreTypeH(size_t hash) const
	{
		const Wiwa::Array<const Type*, TYPE_COUNT>* types = GET_TYPES();

		const Type* type = NULL;

		for (size_t i = 0; i < TYPE_COUNT; i++) {
			if (types->at(i)->hash == hash) {
				type = types->at(i);
				break;
			}
		}

		return type;
	}

	const Type* Application::GetComponentTypeH(size_t hash) const
	{
		size_t size = m_ComponentTypes.size();

		const Type* type = NULL;

		for (size_t i = 0; i < size; i++) {
			if (m_ComponentTypes[i]->hash == hash) {
				type = m_ComponentTypes[i];
				break;
			}
		}

		return type;
	}

	const Type* Application::GetComponentType(size_t index) const
	{
		return m_ComponentTypes[index];
	}

	void Application::RegisterComponentType(const Type* component)
	{
		const Type* type = GetComponentTypeH(component->hash);

		if (!type) m_ComponentTypes.push_back(component);
	}

	void Application::OpenDir(const char* path)
	{
		ShellExecuteA(0, "open", path, NULL, NULL, SW_SHOWNORMAL);
	}

	void Application::Quit()
	{
		OnSaveEvent event;
		OnEvent(event);
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnLoad(OnLoadEvent& e)
	{
		JSONDocument config("config/application.json");

		if (config.HasMember("vsync"))
			m_Window->SetVSync(config["vsync"].get<bool>());

		if (config.HasMember("fullscreen"))
			m_Window->SetVSync(config["fullscreen"].get<bool>());

		if (config.HasMember("resizable"))
			m_Window->SetVSync(config["resizable"].get<bool>());

		return false;
	}
	bool Application::OnSave(OnSaveEvent& e)
	{
		JSONDocument config;

		config.AddMember("vsync", m_Window->IsVSync());
		config.AddMember("fullscreen", m_Window->GetFullScreen());
		config.AddMember("resizable", m_Window->GetResizable());

		config.save_file("config/application.json");

		JSONDocument project;
		project.AddMember("name", m_ProjectName.c_str());
		project.AddMember("version", m_ProjectVersion.c_str());
		project.AddMember("company", m_ProjectCompany.c_str());
		
		project.AddMember("target", (int)m_ProjectTarget);

		project.save_file("config/project.json");

		return false;
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>({ &Application::OnWindowClose, this });
		dispatcher.Dispatch<OnSaveEvent>({ &Application::OnSave, this });
		dispatcher.Dispatch<OnLoadEvent>({ &Application::OnLoad, this });

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