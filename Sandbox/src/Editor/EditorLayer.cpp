#include "EditorLayer.h"

#include <imgui.h>
#include <Wiwa/Application.h>

EditorLayer::EditorLayer()
	: Layer("Editor Layer")
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate()
{
}

void EditorLayer::OnImGuiRender()
{
	ImGuiContext* ctx = Wiwa::Application::Get().GetImGuiContext();
	ImGui::SetCurrentContext(ctx);

	ImGui::Begin("FrameTime");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	ImGui::BeginMainMenuBar();
	if (ImGui::Button("Demo"))
	{
		m_ShowDemo = !m_ShowDemo;
	}
	ImGui::EndMainMenuBar();
	static bool show = true;
	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&show);
}

void EditorLayer::OnEvent(Wiwa::Event& event)
{
}
