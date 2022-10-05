#pragma once
#include <optick.h>
#ifdef WI_PLATFORM_WINDOWS
extern Wiwa::Application* Wiwa::CreateApplication();

int main(int argc, char** argv)
{
	Wiwa::Log::Init();
	WI_CORE_WARN("Initialized Log!");

	auto app = Wiwa::CreateApplication();
	app->Run();

	delete app;
}
#endif // ZO_PLATFORM_WINDOWS
