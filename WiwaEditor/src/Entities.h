#pragma once

#include <Wiwa/core/Application.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
void CreateNew3DEnt()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	EntityId myEntity = em.CreateEntity("New Entity");
	em.AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
}
void CreateNewChild(EntityId parentId)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	EntityId myEntity = em.CreateEntity("New entity", parentId);
	em.AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
}
void CreateCube()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Cube");
	em.AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	Wiwa::Mesh mesh;
	sprintf(mesh.mesh_path, "%s", "cube");
	sprintf(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}
void CreatePlane()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Plane");
	em.AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	Wiwa::Mesh mesh;
	sprintf(mesh.mesh_path, "%s", "plane");
	sprintf(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}
void CreatePyramid()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Pyramid");
	em.AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	Wiwa::Mesh mesh;
	sprintf(mesh.mesh_path, "%s", "pyramid");
	sprintf(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}
void CreateSphere()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Sphere");
	em.AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	Wiwa::Mesh mesh;
	sprintf(mesh.mesh_path, "%s", "resources/meshes/sphere.fbx");
	sprintf(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}