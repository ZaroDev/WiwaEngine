#include <wipch.h>

#include "MeshRenderer.h"
#include <Wiwa/utilities/Log.h>
#include <Wiwa/core/Renderer3D.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/Material.h>

namespace Wiwa {
	MeshRenderer::MeshRenderer()
	{
	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::OnUpdate(EntityId eid)
	{
		Transform3D* t3d = GetComponent<Transform3D>(eid);
		Mesh* mesh = GetComponent<Mesh>(eid);

		Renderer3D& r3d = Application::Get().GetRenderer3D();

		Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
		Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(mesh->materialId);
		if (mat->getType() == Wiwa::Material::MaterialType::color)
			r3d.RenderMeshColor(mod, t3d->position, t3d->rotation, t3d->scale, mat);

		if (mat->getType() == Wiwa::Material::MaterialType::textured)
			r3d.RenderMeshMaterial(mod, t3d->position, t3d->rotation, t3d->scale, mat);
	}
}