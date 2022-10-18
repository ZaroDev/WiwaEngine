#include <wipch.h>

#include "MeshRenderer.h"
#include <Wiwa/Log.h>
#include <Wiwa/Renderer3D.h>

#include <Wiwa/ecs/EntityManager.h>

namespace Wiwa {
	MeshRenderer::MeshRenderer()
	{
	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::OnUpdate()
	{
		std::vector<Transform3D*>* t3dv = GetComponents<Transform3D>();
		std::vector<Mesh*>* sprv = GetComponents<Mesh>();

		size_t size = t3dv->size();
		Transform3D** t3d = t3dv->data();
		Mesh** spr = sprv->data();

		Renderer3D& r3d = Application::Get().GetRenderer3D();
		Size2i& res = Application::Get().GetTargetResolution();

		for (size_t i = 0; i < size; i++)
		{
			Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(spr[i]->id);
			Color4f col = Color::WHITE;
			r3d.RenderMeshColor(*mod, t3d[i]->position, t3d[i]->rotation, t3d[i]->rotation, col);
		}
	}

}