#include <Wiwa.h>

#include "Layers/EditorLayer.h"

const size_t TYPE_COUNT = __COUNTER__;

class WiwaApp : public Wiwa::Application
{
public:
	WiwaApp() 
	{
		PushOverlay(new EditorLayer());

		const Wiwa::Array<const Type*, TYPE_COUNT>* arr = GET_TYPES();

		for (size_t i = 0; i < arr->Size(); i++) {
			printf("%s, ", arr->at(i)->name);
		}
	}

	~WiwaApp()
	{

	}
};

Wiwa::Application* Wiwa::CreateApplication()
{
	return new WiwaApp();
}