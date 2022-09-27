#include <Wiwa.h>

#include "Layers/EditorLayer.h"
#include "Layers/HardwareLayer.h"

class WiwaApp : public Wiwa::Application
{
public:
	WiwaApp() 
	{
		PushOverlay(new EditorLayer());
		PushOverlay(new HardwareLayer());
	}
	~WiwaApp()
	{

	}
};
Wiwa::Application* Wiwa::CreateApplication()
{
	return new WiwaApp();
}
