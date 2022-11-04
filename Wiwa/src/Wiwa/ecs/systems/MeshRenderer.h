#pragma once
#pragma warning(disable : 4267)
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/Mesh.h"

#include <Wiwa/Reflection.h>

namespace Wiwa {
	class WI_API MeshRenderer : public System<Transform3D, Mesh> {
	private:

	public:
		MeshRenderer();
		~MeshRenderer();

		void OnUpdate() override;

		void OnUpdateComponents(Transform3D* t3d, Mesh* mesh) override;
	};
}

//REGISTER_TYPE(Wiwa::MeshRenderer);