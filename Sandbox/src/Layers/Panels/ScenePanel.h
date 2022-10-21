#pragma once

#include "Panel.h"

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/render/Camera.h>
#include <glm.hpp>

struct ShadingView;

class ScenePanel : public Panel
{
public:
	ScenePanel();
	virtual ~ScenePanel();

	void Draw() override;

	inline void SetGizmoType(int type) { m_GizmoType = type; }

private:
	std::vector<ShadingView*> m_Shadings;

	//std::unique_ptr<Wiwa::FrameBuffer> m_FrameBuffer;
	Wiwa::Vector3f m_MeshPosition;
	Wiwa::Vector3f m_MeshRotation;
	Wiwa::Vector3f m_MeshScale;
	Wiwa::Camera m_Camera;

	bool m_ShowFPS = true;

	Wiwa::Vector4f m_MeshColor;

	int m_GizmoType = -1;

	// Camera control
	float camSpeed;
	float sensitivity;

	float yaw;
	float pitch;

	Wiwa::Vector2f lastPos;

	bool m_FirstClick = false;
};
