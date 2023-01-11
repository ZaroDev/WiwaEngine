#pragma once

#include "Panel.h"
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>

#include "../../Events/PanelEvents.h"
#include <Wiwa/Events/MouseEvent.h>

struct ShadingView;

class MeshViewPanel : public Panel
{
public:
	MeshViewPanel(EditorLayer* instance);
	virtual ~MeshViewPanel();

	void Update() override;
	void Draw() override;

	void OnEvent(Wiwa::Event& e);
	bool EntityChange(EntityChangeEvent& e);
	bool OnMouseScrollEvent(Wiwa::MouseScrolledEvent& e);

private:
	std::vector<ShadingView *> m_Shadings;
	Wiwa::Camera m_Camera;

	Wiwa::Model *m_ActiveMesh = nullptr;

	Wiwa::Vector3f m_MeshPosition;
	Wiwa::Vector3f m_MeshRotation;
	Wiwa::Vector3f m_MeshScale;

	Wiwa::Vector4f m_MeshColor;

	Wiwa::Material* m_ActiveMaterial = nullptr;

	// Camera control
	float camSpeed;
	float sensitivity;
	float camFastSpeed;
	float yaw;
	float pitch;

	float m_Scroll = 0.0f;

	Wiwa::Vector2f lastPos;
};