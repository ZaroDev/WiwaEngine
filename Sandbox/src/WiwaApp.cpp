#include <Wiwa.h>

#include "Editor/EditorLayer.h"

class WiwaApp : public Wiwa::Application
{
public:
	WiwaApp() 
	{
		PushOverlay(new EditorLayer());
	}
	~WiwaApp()
	{

	}
};
Wiwa::Application* Wiwa::CreateApplication()
{
	return new WiwaApp();
}
