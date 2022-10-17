
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
#include "ecs/components/Transform3D.h"

REFLECTION_BEGIN(Wiwa::Transform3D)
REFLECT_MEMBER(position)
REFLECT_MEMBER(rotation)
REFLECT_MEMBER(scale)
REFLECTION_END;

const size_t TYPE_COUNT = __COUNTER__;

namespace Wiwa {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		WI_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_TargetResolution = { 1920, 1080 };
		
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		int min, major, rev;
		glfwGetVersion(&min, &major, &rev);
		sprintf_s(m_SysInfo.glfwVer, 32, "%i.%i.%i", min, major, rev);

		SetHwInfo();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_EntityManager = new EntityManager();
		PushLayer(m_EntityManager);

		m_Renderer2D = new Renderer2D();
		m_Renderer2D->Init();

		m_Renderer3D = new Renderer3D();
		m_Renderer3D->Init();

		m_RenderColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		// test
		// TODO: REMOVE TEST
		ResourceId tree = Resources::Load<Image>("resources/images/tree.png");

		m_EntityManager->RegisterSystem<SpriteRenderer>();

		m_EntityManager->ReserveEntities(MAXQUADS);
		//m_EntityManager->ReserveSystem<SpriteRenderer>(MAXQUADS);
		//m_EntityManager->ReserveComponent<Sprite>(MAXQUADS);
		//m_EntityManager->ReserveComponent<Transform2D>(MAXQUADS);
		
		Image* spr = Resources::GetResourceById<Image>(tree);
		Size2i size = spr->GetSize();
		EntityId EntityMyTree = m_EntityManager->CreateEntity();
		m_EntityManager->AddComponent<Transform2D>(EntityMyTree, { {0,0},0.f,{1.0,1.0} });
		m_EntityManager->AddComponent<Sprite>(EntityMyTree, { {256,256}, tree,{size.w / 4, size.h / 4, size.w / 2, size.h / 2} });
		//for (int i = 0; i < MAXQUADS; i++) {
		//	EntityId EntityMyTree = m_EntityManager->CreateEntity();
		//	int x = (i * 32) % m_TargetResolution.w;
		//	int y = (i * 32) % m_TargetResolution.h;

		//	/*entityManager->AddComponent<Sprite>(EntityMyTree, { {32,32}, KawaiTree,{size.w / 4, size.h / 4, size.w / 2, size.h / 2} });
		//	entityManager->AddComponent<Transform2D>(EntityMyTree, { {x,y},0.f,{1.0,1.0} });*/

		//	/*m_EntityManager->AddComponents<Sprite, Transform2D>(EntityMyTree,
		//		{ {32,32}, tree,{size.w / 4, size.h / 4, size.w / 2, size.h / 2 }, Renderer2D::Pivot::UPLEFT },
		//		{ {x,y},0.f,{1.0,1.0} }
		//	);*/

		//	m_EntityManager->AddComponents<Sprite, Transform2D>(EntityMyTree,
		//		{ {32,32}, tree,{0, 0, size.w, size.h }, Renderer2D::Pivot::UPLEFT },
		//		{ {x,y},0.0f,{1.0,1.0} }
		//	);
		//}
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
		m_Running = false;
	
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