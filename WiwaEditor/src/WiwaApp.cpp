#include <Wiwa.h>

#include "Layers/EditorLayer.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/scripting/ScriptEngine.h>

#include <Wiwa/audio/Audio.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/core/Renderer3D.h>

USE_REFLECTION;
class WiwaApp : public Wiwa::Application
{
public:
	REFLECTION_BODY_INL;

	WiwaApp(int argc, char** argv) : Wiwa::Application(argc, argv)
	{
		REFLECTION_REGISTER();

		PushOverlay(new EditorLayer());

		ResourceId imgid = Wiwa::Resources::Load<Wiwa::Image>("assets/Textures/Building_V01_C.png");

		Wiwa::Image* img = Wiwa::Resources::GetResourceById<Wiwa::Image>(imgid);

		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		Wiwa::Vector2i pos = { 0,0 };
		Wiwa::Size2i size = img->GetSize();

		r2d.CreateInstancedQuadTex(img->GetTextureId(), img->GetSize(), pos, size, Wiwa::Renderer2D::Pivot::UPLEFT);
	}

	~WiwaApp()
	{

	}
};

Wiwa::Application* Wiwa::CreateApplication(int argc, char** argv)
{
	return new WiwaApp(argc, argv);
}