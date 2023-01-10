#include <Wiwa.h>

#include "Layers/EditorLayer.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/scripting/ScriptEngine.h>

#include <Wiwa/audio/Audio.h>

USE_REFLECTION;
class WiwaApp : public Wiwa::Application
{
public:
	REFLECTION_BODY_INL;

	WiwaApp(int argc, char** argv) : Wiwa::Application(argc, argv)
	{
		REFLECTION_REGISTER();

		PushOverlay(new EditorLayer());

		bool res = Audio::LoadProject("assets/wwise/Wwise_Test/GeneratedSoundBanks/Windows/Init.bnk");

		if (!res) {
			WI_CORE_ERROR("Audio load project error: [{}]", Audio::GetLastError());
		}

		Audio::LoadBank("assets/wwise/Wwise_Test/GeneratedSoundBanks/Windows/Ruido.bnk");

		if (!res) {
			WI_CORE_ERROR("Audio load bank error: [{}]", Audio::GetLastError());
		}

		Audio::PostEvent("Ruido", 25);
		Audio::Update();

		const Type* type = Wiwa::ScriptEngine::getSystems()[FNV1A_HASH("EnemyController")];

		Wiwa::System* system = (Wiwa::System*)type->New();
		system->AddEntity(0);
		system->Awake();
		system->Init();
		system->Update();

		delete system;
	}

	~WiwaApp()
	{

	}
};

Wiwa::Application* Wiwa::CreateApplication(int argc, char** argv)
{
	return new WiwaApp(argc, argv);
}