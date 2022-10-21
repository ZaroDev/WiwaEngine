#pragma once

#include <Wiwa/Application.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/ecs/components/Mesh.h>

void CreateCube()
{
	EntityId myEntity = Wiwa::Application::Get().GetEntityManager().CreateEntity("Cube");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	ResourceId id = Wiwa::Resources::Load<Wiwa::Model>("cube");
	ResourceId matId = Wiwa::Resources::Load<Wiwa::Material>("resources/materials/default_mat.wimaterial");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Mesh>(myEntity, { id, matId });
}
void CreatePlane()
{
	EntityId myEntity = Wiwa::Application::Get().GetEntityManager().CreateEntity("Plane");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	ResourceId id = Wiwa::Resources::Load<Wiwa::Model>("plane");
	ResourceId matId = Wiwa::Resources::Load<Wiwa::Material>("resources/materials/default_mat.wimaterial");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Mesh>(myEntity, { id, matId });
}
void CreatePyramid()
{
	EntityId myEntity = Wiwa::Application::Get().GetEntityManager().CreateEntity("Pyramid");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	ResourceId id = Wiwa::Resources::Load<Wiwa::Model>("pyramid");
	ResourceId matId = Wiwa::Resources::Load<Wiwa::Material>("resources/materials/default_mat.wimaterial");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Mesh>(myEntity, { id, matId });
}
void CreateSphere()
{
	EntityId myEntity = Wiwa::Application::Get().GetEntityManager().CreateEntity("Sphere");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	ResourceId id = Wiwa::Resources::Load<Wiwa::Model>("sphere");
	ResourceId matId = Wiwa::Resources::Load<Wiwa::Material>("resources/materials/default_mat.wimaterial");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Mesh>(myEntity, { id, matId });
}