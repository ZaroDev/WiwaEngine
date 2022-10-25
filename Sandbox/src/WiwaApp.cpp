#include <Wiwa.h>

#include "Layers/EditorLayer.h"

const size_t TYPE_COUNT = __COUNTER__;

class WiwaApp : public Wiwa::Application
{
public:
	size_t getAppTypeCount() const override {
		return TYPE_COUNT;
	}

	const Type* getAppType(size_t index) const override {
		return GET_TYPES()->at(index);
	}

	const Type* getAppTypeH(size_t hash) const override {
		const Wiwa::Array<const Type*, TYPE_COUNT>* types = GET_TYPES();

		const Type* type = NULL;

		for (size_t i = 0; i < TYPE_COUNT; i++) {
			if (types->at(i)->hash == hash) {
				type = types->at(i);
				break;
			}
		}

		return type;
	}

	WiwaApp(int argc, char** argv) : Wiwa::Application(argc, argv)
	{
		PushOverlay(new EditorLayer());
	}

	~WiwaApp()
	{

	}
};

Wiwa::Application* Wiwa::CreateApplication(int argc, char** argv)
{
	return new WiwaApp(argc, argv);
}