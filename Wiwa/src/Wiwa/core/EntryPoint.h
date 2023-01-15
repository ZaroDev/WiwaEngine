#pragma once

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Application.h>

#ifdef WI_PLATFORM_WINDOWS
extern Wiwa::Application* Wiwa::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	Wiwa::Log::Init();
	WI_CORE_INFO("Initialized Log!");

	auto app = Wiwa::CreateApplication(argc, argv);
	app->Run();

	delete app;
}
#endif // WI_PLATFORM_WINDOWS