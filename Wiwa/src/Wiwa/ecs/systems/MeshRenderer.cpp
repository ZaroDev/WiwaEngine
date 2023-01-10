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

		if (!mesh) return;

		Renderer3D& r3d = Application::Get().GetRenderer3D();

		Model* root_mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
		Model* mod = root_mod;

		if (root_mod->IsRoot()) {
			if(!mesh->drawChildren)
				mod = root_mod->getModelAt(mesh->modelIndex);
		}
		
		Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(mesh->materialId);

		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		

		size_t cameraCount = man.getCameraSize();
		std::vector<CameraId>& cameras = man.getCameras();
		for (size_t i = 0; i < cameraCount; i++)
		{
			CameraId cam_id = cameras[i];
			Camera* camera = man.getCamera(cam_id);

			if (camera->cull &&
				!camera->frustrum.IsBoxVisible(mod->boundingBox.getMin(), mod->boundingBox.getMax())
			) return;
			r3d.RenderMeshMaterial(mod, t3d->position, t3d->rotation, t3d->scale, mat, false, man.editorCamera);

			r3d.RenderMeshMaterial(mod, t3d->position, t3d->rotation, t3d->scale, mat, false, camera);
		}
	}
}