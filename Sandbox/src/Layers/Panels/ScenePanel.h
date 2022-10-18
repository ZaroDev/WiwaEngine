#pragma once

#include "Panel.h"

#include <Wiwa/utilities/math/Math.h>
#include <glm.hpp>

struct ShadingView;

namespace Wiwa
{
	class FrameBuffer;
	class Camera;
	class Model;
};

class ScenePanel : public Panel
{
public:
	ScenePanel();
	virtual ~ScenePanel();

	void Draw() override;

	inline void SetGizmoType(int type) { m_GizmoType = type; }

private:
	std::vector<ShadingView*> m_Shadings;

	std::unique_ptr<Wiwa::FrameBuffer> m_FrameBuffer;
	std::unique_ptr<Wiwa::Camera> m_Camera;
	Wiwa::Vector3f m_MeshPosition;
	Wiwa::Vector3f m_MeshRotation;
	Wiwa::Vector3f m_MeshScale;

	bool m_ShowFPS = true;

	Wiwa::Vector4f m_MeshColor;

	int m_GizmoType = -1;
};
