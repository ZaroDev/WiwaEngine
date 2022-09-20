#include <Wiwa.h>

class ExampleLayer : public Wiwa::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		//ZO_INFO("ExampleLayer::Update");
	}

	void OnEvent(Wiwa::Event& event) override
	{
		//ZO_TRACE("{0}", event);
	}
};

class Sandbox : public Wiwa::Application
{
public:
	Sandbox() 
	{
		//PushLayer(new ExampleLayer());
		PushOverlay(new Wiwa::ImGuiLayer());
	}
	~Sandbox()
	{

	}
};
Wiwa::Application* Wiwa::CreateApplication()
{
	return new Sandbox();
}
