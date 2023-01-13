#pragma once

#include "Panel.h"

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/render/Camera.h>
#include <glm/glm.hpp>
#include <Wiwa/Events/MouseEvent.h>
#include <Wiwa/ecs/components/Transform3D.h> 
#include <Wiwa/utilities/render/CameraManager.h>

struct ShadingView;

class ScenePanel : public Panel
{
public:
	ScenePanel(EditorLayer* instance);
	virtual ~ScenePanel();

	void Draw() override;
	void Update() override;
	void OnEvent(Wiwa::Event& e) override;
	bool OnMouseScrollEvent(Wiwa::MouseScrolledEvent& e);
	bool OnEntityChange(EntityChangeEvent& e);

	Wiwa::Vector2f GetMousePosition() { return lastPos; }
private:
	std::vector<ShadingView*> m_Shadings;

	Wiwa::Camera* m_Camera;
	CameraId m_CameraId = -1;
	
	bool m_ShowFPS = true;

	Wiwa::Vector4f m_MeshColor = {};

	int m_GizmoType = -1;

	float camSpeed;
	float camFastSpeed;
	float sensitivity;
	float nearPlane;
	float farPlane;

	float yaw;
	float pitch;

	float m_ScrollSpeed;
	float m_Scroll = 0.0f;

	Wiwa::Vector2f lastPos = {};
	
	Wiwa::Transform3D* m_SelectedTransform;

	int m_EntSelected = -1;
	bool m_LocalMode;
};
