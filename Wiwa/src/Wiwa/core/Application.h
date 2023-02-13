#pragma once

#pragma warning(disable : 4251)

#include "Core.h"
#include "Window.h"
#include <Wiwa/core/LayerStack.h>
#include "Wiwa/Events/Event.h"
#include "Wiwa/Events/ApplicationEvent.h"
#include "Wiwa/ImGui/ImGuiLayer.h"

#include <Wiwa/utilities/math/Vector2i.h>
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/math/Vector4f.h>
#include <Wiwa/utilities/time/Time.h>

namespace Wiwa
{
	class SceneManager;
	class Renderer2D;
	class Renderer3D;

	struct SysInfo
	{
		SysInfo() = default;

		char glfwVer[32];
		uint32_t cache;
		uint32_t numCores;
		float ram;
		const unsigned char *gpu;
		const unsigned char *gpuBrand;
		float gpuVRAM;
		float gpuVRAMUsage;
		float gpuVRAMAV;
		float gpuVRAMReserve;
	};

	enum class ProjectTarget
	{
		None = 0,
		Windows
	};

	class WI_API Application
	{
	public:
		Application(int argc, char** argv);
		~Application();

		void Run();

		void OnEvent(Event &e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);

		void SetHwInfo();

		inline static Application &Get() { return *s_Instance; }

		inline int GetArgC() { return m_ArgC; }
		inline const char* GetArgv(int index) { return m_Argv[index].c_str(); }

		inline Window &GetWindow() { return *m_Window; }
		inline Renderer2D &GetRenderer2D() { return *m_Renderer2D; }
		inline Renderer3D &GetRenderer3D() { return *m_Renderer3D; }
		inline ImGuiLayer &GetImGuiLayer() { return *m_ImGuiLayer; }

		inline ImGuiContext *GetImGuiContext() { return m_ImGuiLayer->GetContext(); }

		inline SysInfo &GetSystemInfo() { return m_SysInfo; }
		inline const char *GetVersion() { return m_EngineVersion; }

		inline const char* GetProjectName() { return m_ProjectName.c_str(); }
		inline void SetProjectName(const char* name) { m_ProjectName = name; }
		inline const char* GetProjectCompany() { return m_ProjectCompany.c_str(); }
		inline void SetProjectCompany(const char* name) { m_ProjectCompany = name; }
		inline const char* GetProjectVersion() { return m_ProjectVersion.c_str(); }
		inline void SetProjectVersion(const char* name) { m_ProjectVersion = name; }

		inline ProjectTarget& GetProjectTarget() { return m_ProjectTarget; }

		inline void SetTargetResolution(int width, int height) { m_TargetResolution = {width, height}; }
		inline Size2i &GetTargetResolution() { return m_TargetResolution; }

		// Core Type Reflection
		size_t getCoreTypeCount() const;
		const Type *getCoreType(size_t index) const;
		const Type *getCoreTypeH(size_t hash) const;
		// App Type Reflection
		virtual size_t getAppTypeCount() const = 0;
		virtual const Type* getAppType(size_t index) const = 0;
		virtual const Type* getAppTypeH(size_t hash) const = 0;
		// Component reflection
		size_t GetComponentTypeCount() const { return m_ComponentTypes.size(); }
		const Type* GetComponentTypeH(size_t hash) const;
		const Type* GetComponentType(size_t index) const;
		void RegisterComponentType(const Type* component);
		void DeleteComponentType(const Type* component);
		// System reflection
		size_t GetSystemTypeCount() const { return m_SystemTypes.size(); }
		const Type* GetSystemTypeH(size_t hash) const;
		const Type* GetSystemType(size_t index) const;
		bool HasSystemH(size_t hash) const;

		void RegisterSystemType(const Type* system);
		void DeleteSystemType(const Type* system);

		inline void SetRenderColor(Vector4f color) { m_RenderColor = color; }
		inline Vector4f GetRenderColor() { return m_RenderColor; }

		void OpenDir(const char *url);
		void Quit();

		void SubmitToMainThread(const std::function<void()> func);

	private:
		void ExecuteMainThreadQueue();
	private:
		int m_ArgC;
		std::vector<std::string> m_Argv;

		std::vector<const Type*> m_ComponentTypes;
		std::vector<const Type*> m_SystemTypes;

		Size2i m_TargetResolution;

		bool OnWindowClose(WindowCloseEvent &e);
		bool OnLoad(OnLoadEvent &e);
		bool OnSave(OnSaveEvent &e);

		const char *m_EngineVersion = "0.3";
		std::string m_ProjectName = "New project";
		std::string m_ProjectCompany = "Default Company";
		std::string m_ProjectVersion = "0.1";
		
		ProjectTarget m_ProjectTarget = ProjectTarget::Windows;

		Renderer2D *m_Renderer2D;
		Renderer3D *m_Renderer3D;
		Vector4f m_RenderColor;

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		SysInfo m_SysInfo;

		LayerStack m_LayerStack;
		ImGuiLayer *m_ImGuiLayer;

		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;

	private:
		static Application *s_Instance;
	};

	// To be defined in a client
	Application *CreateApplication(int argc, char** argv);
}

#define USE_REFLECTION const size_t TYPE_COUNT = __COUNTER__;

#define REFLECTION_BODY_INL \
	size_t getAppTypeCount() const override { return TYPE_COUNT; } \
	const Type* getAppType(size_t index) const override { return GET_TYPES()->at(index); } \
	const Type* getAppTypeH(size_t hash) const override { \
		const Wiwa::Array<const Type*, TYPE_COUNT>* types = GET_TYPES(); \
		const Type* type = NULL; \
		for (size_t i = 0; i < TYPE_COUNT; i++) { \
			if (types->at(i)->hash == hash) { \
				type = types->at(i); \
				break; \
			} \
		} \
		return type; \
	}

#define REFLECTION_REGISTER() \
	const Wiwa::Array<const Type*, TYPE_COUNT>* types = GET_TYPES(); \
	for(size_t i=0;i<TYPE_COUNT;i++){ if(types->at(i)->custom_id == 0) RegisterComponentType(types->at(i)); else RegisterSystemType(types->at(i)); }