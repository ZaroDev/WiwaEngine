#pragma once

#ifdef WI_PLATFORM_WINDOWS
extern Wiwa::Application* Wiwa::CreateApplication();

extern const int TYPE_COUNT = __COUNTER__;

int main(int argc, char** argv)
{
	Wiwa::Log::Init();
	WI_CORE_WARN("Initialized Log!");

	auto app = Wiwa::CreateApplication();
	app->Run();

	delete app;
}
#endif // ZO_PLATFORM_WINDOWS
