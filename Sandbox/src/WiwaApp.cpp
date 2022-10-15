#include <Wiwa.h>

#include "Layers/EditorLayer.h"

struct rtest {
	int x, y;
};

class WiwaApp : public Wiwa::Application
{
public:
	WiwaApp() 
	{
		PushOverlay(new EditorLayer());

		//Wiwa::Array<const Type*, TYPE_COUNT> arr = GET_TYPES();

		/*GET_TYPES();*/

		size_t test = 0;
		const Type* t = GetCompileType<1>();

		const Type* typesito = GetType<int>();

		printf("%u", test);
	}

	~WiwaApp()
	{

	}
};

Wiwa::Application* Wiwa::CreateApplication()
{
	return new WiwaApp();
}