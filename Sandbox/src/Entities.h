#pragma once

#include <Wiwa/Application.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/ecs/components/Transform3D.h>

#include <Wiwa/utilities/render/primitives/Cube.h>
#include <Wiwa/utilities/render/primitives/Plane.h>
#include <Wiwa/utilities/render/primitives/Pyramid.h>
#include <Wiwa/utilities/render/primitives/Sphere.h>

Wiwa::Mesh* CreateCube()
{
	EntityId myEntity = Wiwa::Application::Get().GetEntityManager().CreateEntity("Cube");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	Wiwa::Cube* cube = new Wiwa::Cube();

	return cube;
}
Wiwa::Mesh* CreatePlane()
{
	EntityId myEntity = Wiwa::Application::Get().GetEntityManager().CreateEntity("Plane");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	Wiwa::Plane* plane = new Wiwa::Plane();

	return plane;
}
Wiwa::Mesh* CreatePyramid()
{
	EntityId myEntity = Wiwa::Application::Get().GetEntityManager().CreateEntity("Pyramid");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	Wiwa::Pyramid* pyramid = new Wiwa::Pyramid();
	return pyramid;
}
Wiwa::Mesh* CreateSphere()
{
	EntityId myEntity = Wiwa::Application::Get().GetEntityManager().CreateEntity("Sphere");
	Wiwa::Application::Get().GetEntityManager().AddComponent<Wiwa::Transform3D>(myEntity, { {0,0,0},{0.0f, 0.0f, 0.0f},{1.0,1.0, 1.0} });
	Wiwa::Sphere* sphere = new Wiwa::Sphere();
	return sphere;
}