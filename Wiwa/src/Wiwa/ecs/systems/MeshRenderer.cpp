#include <wipch.h>

#include "MeshRenderer.h"
#include <Wiwa/utilities/Log.h>
#include <Wiwa/core/Renderer3D.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/render/CameraManager.h>
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
		size_t cameraCount = Wiwa::CameraManager::getCameraSize();
		for (size_t i = 0; i < cameraCount; i++)
		{
			if (Wiwa::CameraManager::getCamera(i)->cull && !Wiwa::CameraManager::getCamera(i)->frustrum.IsBoxVisible(mod->boundingBox.getMin(), mod->boundingBox.getMax()))
				return;
			r3d.RenderMeshMaterial(mod, t3d->position, t3d->rotation, t3d->scale, mat, false, Wiwa::CameraManager::getCamera(i));
			r3d.RenderMeshMaterial(mod, t3d->position, t3d->rotation, t3d->scale, mat, false, Wiwa::CameraManager::editorCamera);

		}
	}
}