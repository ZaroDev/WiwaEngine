
#include "wipch.h"
#include "Application.h"

#include <Wiwa/ecs/EntityManager.h>

#include "Wiwa/Log.h"

#include <glew.h>
#include <GLFW/glfw3.h>

#include "Input.h"

#include <shellapi.h>
#include <Windows.h>

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#include "Renderer2D.h"
#include "Renderer3D.h"

#include "ecs/systems/SpriteRenderer.h"
#include "ecs/systems/MeshRenderer.h"

#include "ecs/components/Transform3D.h"

#include "scene/SceneManager.h"

#include "utilities/json/JSONDocument.h"

const size_t TYPE_COUNT = __COUNTER__;

namespace Wiwa {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		WI_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_TargetResolution = { 1920, 1080 };
		
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback({ &Application::OnEvent, this });

		int min, major, rev;
		glfwGetVersion(&min, &major, &rev);
		sprintf_s(m_SysInfo.glfwVer, 32, "%i.%i.%i", min, major, rev);

		SetHwInfo();

		m_Renderer2D = new Renderer2D();
		m_Renderer2D->Init();

		m_Renderer3D = new Renderer3D();
		m_Renderer3D->Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_SceneManager = new SceneManager();
		PushLayer(m_SceneManager);

		m_EntityManager = new EntityManager();

		m_RenderColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		m_EntityManager->RegisterSystem<SpriteRenderer>();
		m_EntityManager->RegisterSystem<MeshRenderer>();

		m_EntityManager->ReserveComponent<Mesh>(10);
		m_EntityManager->ReserveComponent<Transform3D>(10);

		Mesh mesh;
		mesh.meshId = Resources::Load<Model>("resources/meshes/BakerHouse.fbx");
		mesh.materialId = Resources::Load<Material>("resources/materials/bakerhouse_material.wimaterial");

		Transform3D t3d;
		t3d.position = { 0.0f, 0.0f, 0.0f };
		t3d.rotation = { 0.0f,0.0f, 0.0f };
		t3d.scale = { 1.0f, 1.0f, 1.0f };

		EntityId eid = m_EntityManager->CreateEntity();
		m_EntityManager->AddComponent<Mesh>(eid, mesh);
		m_EntityManager->AddComponent<Transform3D>(eid, t3d);

		EntityId oid = m_EntityManager->CreateEntity();
		m_EntityManager->AddComponent<Mesh>(oid, mesh);
		m_EntityManager->AddComponent<Transform3D>(oid, t3d);

		EntityId aid = m_EntityManager->CreateEntity();
		m_EntityManager->AddComponent<Mesh>(aid, mesh);
		m_EntityManager->AddComponent<Transform3D>(aid, t3d);

		Mesh mesh2;
		mesh2.meshId = Resources::Load<Model>("resources/meshes/cube.fbx");
		mesh2.materialId = Resources::Load<Material>("Assets/textures/test.wimaterial");
		Transform3D t3d2;
		t3d2.position = { 2.0f, 0.0f, 0.0f };
		t3d2.rotation = { 0.0f,0.0f, 0.0f };
		t3d2.scale = { 1.0f, 1.0f, 1.0f };

		EntityId eid2 = m_EntityManager->CreateEntity();
		m_EntityManager->AddComponent<Mesh>(eid2, mesh2);
		m_EntityManager->AddComponent<Transform3D>(eid2, t3d2);


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

	}

	void Application::Run()
	{
		while (m_Running)
		{
			OPTICK_FRAME("Application Loop");
			OPTICK_EVENT();
			glClearColor(m_RenderColor.r, m_RenderColor.g, m_RenderColor.b, m_RenderColor.a);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Renderer2D->Update();
			m_Renderer3D->Update();

			m_Time = (float)glfwGetTime();
			m_EntityManager->Update();

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