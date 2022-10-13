#include <Wiwa.h>

#include "Layers/EditorLayer.h"


class WiwaApp : public Wiwa::Application
{
public:
	WiwaApp() 
	{
		PushOverlay(new EditorLayer());

		Wiwa::Array<const Type*, TYPE_COUNT> arr = GET_TYPES();
	}

	~WiwaApp()
	{

	}
};

Wiwa::Application* Wiwa::CreateApplication()
{
	return new WiwaApp();
}