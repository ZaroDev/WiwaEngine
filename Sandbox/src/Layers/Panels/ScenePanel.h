#pragma once

#include "Panel.h"

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/render/Camera.h>
#include <glm.hpp>
#include <Wiwa/Events/MouseEvent.h>

struct ShadingView;

class ScenePanel : public Panel
{
public:
	ScenePanel();
	virtual ~ScenePanel();

	void Draw() override;

	inline void SetGizmoType(int type) { m_GizmoType = type; }

	void OnEvent(Wiwa::Event& e) override;
	bool OnMouseScrollEvent(Wiwa::MouseScrolledEvent& e);
private:
	std::vector<ShadingView*> m_Shadings;

	Wiwa::Vector3f m_MeshPosition;
	Wiwa::Vector3f m_MeshRotation;
	Wiwa::Vector3f m_MeshScale;
	Wiwa::Camera m_Camera;

	bool m_ShowFPS = true;

	Wiwa::Vector4f m_MeshColor;

	int m_GizmoType = -1;

	float camSpeed;
	float camFastSpeed;
	float sensitivity;

	float yaw;
	float pitch;

	float m_Scroll = 0.0f;

	Wiwa::Vector2f lastPos;
};
