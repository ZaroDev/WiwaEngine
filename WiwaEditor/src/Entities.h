#pragma once

#include <Wiwa/core/Application.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/ecs/components/PointLight.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/ecs/systems/LightSystem.h>

inline void CreateNew3DEnt()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	EntityId myEntity = em.CreateEntity("New Entity");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.localRotation = { 0.0f, 0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };
	t3d.localScale = { 0.0f, 0.0f, 0.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreateLightSource()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	EntityId myEntity = em.CreateEntity("Light source");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.localRotation = { 0.0f, 0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };
	t3d.localScale = { 0.0f, 0.0f, 0.0f };

	Wiwa::PointLight lightSource;
	lightSource.linear = 1.0f;
	lightSource.quadratic = 1.0f;
	lightSource.constant = 1.0f;
	lightSource.ambient = glm::vec3(1.f);
	lightSource.diffuse = glm::vec3(1.f);
	lightSource.specular = glm::vec3(1.f);

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);

	em.AddComponent<Wiwa::PointLight>(myEntity, lightSource);
	em.ApplySystem<Wiwa::LightSystem>(myEntity);
}

inline void CreateNewChild(EntityId parentId)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	EntityId myEntity = em.CreateEntity("New entity", parentId);

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.localRotation = { 0.0f, 0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };
	t3d.localScale = { 0.0f, 0.0f, 0.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreateCube()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Cube");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.localRotation = { 0.0f, 0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };
	t3d.localScale = { 0.0f, 0.0f, 0.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	Wiwa::Mesh mesh;
	sprintf_s(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>("cube");
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	mesh.modelIndex = 0;
	mesh.drawChildren = false;
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreatePlane()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Plane");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.localRotation = { 0.0f, 0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };
	t3d.localScale = { 0.0f, 0.0f, 0.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	Wiwa::Mesh mesh;
	sprintf_s(mesh.mesh_path, "%s", "plane");
	sprintf_s(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	mesh.modelIndex = 0;
	mesh.drawChildren = false;
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreatePyramid()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Pyramid");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.localRotation = { 0.0f, 0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };
	t3d.localScale = { 0.0f, 0.0f, 0.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	Wiwa::Mesh mesh;
	sprintf_s(mesh.mesh_path, "%s", "models/pyramid");
	sprintf_s(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	mesh.modelIndex = 0;
	mesh.drawChildren = false;
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreateSphere()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Sphere");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.localRotation = { 0.0f, 0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };
	t3d.localScale = { 0.0f, 0.0f, 0.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	Wiwa::Mesh mesh;
	sprintf_s(mesh.mesh_path, "%s", "models/sphere");
	sprintf_s(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	mesh.modelIndex = 0;
	mesh.drawChildren = false;
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreateEntityWithModelHierarchy(const char* model_path) {
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	// Prepare mesh data
	Wiwa::Mesh mesh;
	sprintf_s(mesh.mesh_path, "%s", model_path);

	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.drawChildren = false;

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.localRotation = { 0.0f, 0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };
	t3d.localScale = { 0.0f, 0.0f, 0.0f };

	// Take root model
	Wiwa::Model* model = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh.meshId);

	// Create entity with model's hierarchy
	const Wiwa::ModelHierarchy* model_h = model->getModelHierarchy();
	size_t children_size = model_h->children.size();

	EntityId e_root = em.CreateEntity(model_h->name.c_str());
	t3d.localPosition = model_h->translation;
	t3d.localRotation = model_h->rotation;
	t3d.localScale = model_h->scale;
	em.AddComponent<Wiwa::Transform3D>(e_root, t3d);
	em.ApplySystem<Wiwa::MeshRenderer>(e_root);
	em.ApplySystem<Wiwa::AudioSystem>(e_root);

	auto process_h = [&](const Wiwa::ModelHierarchy* m_h, EntityId parent, auto&& process_h) -> void {
		EntityId e_child = em.CreateEntity(m_h->name.c_str(), parent);
		t3d.localPosition = m_h->translation;
		t3d.localRotation = m_h->rotation;
		t3d.localScale = m_h->scale;

		em.AddComponent(e_child, t3d);

		size_t c_mesh_size = m_h->meshIndexes.size();
		size_t c_child_size = m_h->children.size();

		if (c_mesh_size > 0) {
			mesh.modelIndex = m_h->meshIndexes[0];
			
			Wiwa::Model* c_model = model->getModelAt(mesh.modelIndex);
			std::string mat_file = model->getMaterialAt(c_model->getMaterialIndex());
			sprintf_s(mesh.mat_path, "%s", mat_file.c_str());

			mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);

			em.AddComponent(e_child, mesh);
			em.ApplySystem<Wiwa::MeshRenderer>(e_child);
			em.ApplySystem<Wiwa::AudioSystem>(e_child);
		}

		if (c_child_size > 0) {
			for (size_t i = 0; i < c_child_size; i++) {
				process_h(m_h->children[i], e_child, process_h);
			}
		}
	};

	for (size_t i = 0; i < children_size; i++) {
		const Wiwa::ModelHierarchy* child_h = model_h->children[i];

		process_h(child_h, e_root, process_h);
	}
}