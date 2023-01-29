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

	void MeshRenderer::OnUpdate()
	{
		Transform3D* t3d = GetComponent<Transform3D>();
		Mesh* mesh = GetComponent<Mesh>();

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
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		size_t cameraCount = man.getCameraSize();
		std::vector<CameraId>& cameras = man.getCameras();

		EntityId parent = eman.GetEntityParent(m_EntityId);
		Transform3D* parent_t3d = eman.GetComponent<Transform3D>(parent);

		for (size_t i = 0; i < cameraCount; i++)
		{
			CameraId cam_id = cameras[i];
			Camera* camera = man.getCamera(cam_id);

			if (camera->cull && !camera->frustrum.IsBoxVisible(mod->boundingBox.getMin(), mod->boundingBox.getMax()))
				return;

			r3d.RenderMesh(mod, t3d->worldMatrix, mat, false, camera);
		}

		r3d.RenderMesh(mod, t3d->worldMatrix, mat, false, man.editorCamera);
	}
}