#include <wipch.h>

#include "MeshRenderer.h"
#include <Wiwa/Log.h>
#include <Wiwa/Renderer3D.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/Material.h>

namespace Wiwa {
	MeshRenderer::MeshRenderer()
	{
	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::OnUpdate()
	{		
		Transform3D* t3dv = GetComponents<Transform3D>();
		size_t* t3din = GetComponentIndexes<Transform3D>();

		Mesh* meshes = GetComponents<Mesh>();
		size_t* meshesin = GetComponentIndexes<Mesh>();

		size_t rcount = GetRegisteredSize();

		Renderer3D& r3d = Application::Get().GetRenderer3D();
		Size2i& res = Application::Get().GetTargetResolution();

		for (size_t i = 0; i < rcount; i++)
		{
			Transform3D* t3d = t3dv + t3din[i];
			Mesh* mesh = meshes + meshesin[i];

			Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
			Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(mesh->materialId);
			if(mat->getType() == Wiwa::Material::MaterialType::color)
				r3d.RenderMeshColor(mod, t3d->position, t3d->rotation, t3d->scale, mat);

			if (mat->getType() == Wiwa::Material::MaterialType::textured)
				r3d.RenderMeshMaterial(mod, t3d->position, t3d->rotation, t3d->scale, mat);
		}
	}
}