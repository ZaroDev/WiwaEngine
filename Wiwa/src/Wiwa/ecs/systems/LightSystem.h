#pragma once
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/PointLight.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API LightSystem : public System {
	private:

	public:
		LightSystem();
		~LightSystem();

		virtual void OnEntitySet() override;
	};
}

REGISTER_SYSTEM(Wiwa::LightSystem);
