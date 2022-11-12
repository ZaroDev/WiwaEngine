#pragma once
#pragma warning(disable : 4267)
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/Mesh.h"

#include <Wiwa/Reflection.h>

namespace Wiwa {
	class WI_API MeshRenderer : public System {
	private:

	public:
		MeshRenderer();
		~MeshRenderer();

		void OnUpdate(EntityId eid) override;
	};
}

//REGISTER_TYPE(Wiwa::MeshRenderer);