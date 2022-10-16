#include <Wiwa.h>

#include "Layers/EditorLayer.h"

struct kek {
	int caca, pipi, moco;
};

REFLECTION_BEGIN(kek)
	REFLECT_MEMBER(caca)
	REFLECT_MEMBER(pipi)
	REFLECT_MEMBER(moco)
REFLECTION_END;

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