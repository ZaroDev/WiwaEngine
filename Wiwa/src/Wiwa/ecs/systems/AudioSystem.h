#pragma once
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/AudioSource.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API AudioSystem : public System {
	private:

	public:
		AudioSystem();
		~AudioSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;
	};
}

REGISTER_SYSTEM(Wiwa::AudioSystem);