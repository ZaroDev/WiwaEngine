#pragma once

#include "Panel.h"
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>

struct ShadingView;

class MeshViewPanel : public Panel
{
public:
	MeshViewPanel();
	virtual ~MeshViewPanel();

	void Update() override;
	void Draw() override;

private:
	std::vector<ShadingView *> m_Shadings;
	Wiwa::FrameBuffer m_FrameBuffer;
	Wiwa::Camera m_Camera;

	Wiwa::Model *m_ActiveMesh;

	Wiwa::Vector3f m_MeshPosition;
	Wiwa::Vector3f m_MeshRotation;
	Wiwa::Vector3f m_MeshScale;

	Wiwa::Vector4f m_MeshColor;

	Wiwa::Material* m_ActiveMaterial;

	// Camera control
	float camSpeed;
	float sensitivity;

	float yaw;
	float pitch;

	Wiwa::Vector2f lastPos;
};