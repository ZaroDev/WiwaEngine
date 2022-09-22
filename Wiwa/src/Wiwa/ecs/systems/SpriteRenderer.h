#pragma once

#include "System.h"
#include "../components/Transform2D.h"
#include "../components/Sprite.h"

namespace EM {
	class EM_API SpriteRenderer : public System<Transform2D, Sprite> {
	private:

	public:
		SpriteRenderer();
		~SpriteRenderer();

		void OnUpdate() override;

	protected:
		virtual void OnEntityAdded(EntityId entityID) override;
	};
}