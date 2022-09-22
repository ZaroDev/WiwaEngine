#pragma once

#include "System.h"
#include "../components/Transform2D.h"
#include "../components/Player.h"

namespace EM {
	class EM_API PlayerController : public System<Transform2D, Player> {
	private:

	public:
		PlayerController();
		~PlayerController();

		void OnUpdate() override;

	protected:
		virtual void OnEntityAdded(EntityId entityID) override;
	};
}