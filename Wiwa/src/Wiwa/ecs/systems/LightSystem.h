#pragma once
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/LightSource.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API LightSystem : public System {
	private:

	public:
		LightSystem();
		~LightSystem();
	};
}

REGISTER_SYSTEM(Wiwa::LightSystem);
