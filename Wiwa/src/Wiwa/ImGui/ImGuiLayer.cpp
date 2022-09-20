#include "wipch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Wiwa/Platform/OpenGL/imgui_impl_opengl3.h"
#include "Wiwa/Platform/OpenGL/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

#include "Wiwa/Application.h"
#include "Wiwa/KeyCodes.h"

//Temp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Wiwa {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
		io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach()
	{
	}
	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();

		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::BeginMainMenuBar();
		if (ImGui::Button("Demo"))
		{
			m_ShowDemo = !m_ShowDemo;
		}
		ImGui::EndMainMenuBar();
		static bool show = true;
		if(m_ShowDemo)
			ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher  dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(ZO_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(ZO_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(ZO_BIND_EVENT_FN(ImGuiLayer::OnMouseMoveEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(ZO_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(ZO_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(ZO_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(ZO_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ZO_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}
	bool ImGuiLayer::OnMouseMoveEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[Key::LeftControl] || io.KeysDown[Key::RightControl];
		io.KeyAlt = io.KeysDown[Key::LeftAlt] || io.KeysDown[Key::RightAlt];
		io.KeyShift = io.KeysDown[Key::LeftShift] || io.KeysDown[Key::RightShift];
		io.KeySuper = io.KeysDown[Key::LeftSuper] || io.KeysDown[Key::RightSuper];

		return false;
	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);
		return false;
	}
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;
		return false;
	}
	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}