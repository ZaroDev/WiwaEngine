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

		void OnEntityAdded(EntityId eid) override;

		void OnEntityRemoved(EntityId eid) override;

		void OnUpdate(EntityId eid) override;
	};
}