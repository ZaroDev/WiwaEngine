#pragma once

#include "Panel.h"
#include <Wiwa/utilities/math/Math.h>
struct ShadingView;

namespace Wiwa
{
	class FrameBuffer;
	class Camera;
	class Mesh;
};

class MeshViewPanel : public Panel
{
public:
	MeshViewPanel();
	virtual ~MeshViewPanel();

	void Draw() override;

private:
	std::vector<ShadingView *> m_Shadings;
	std::unique_ptr<Wiwa::FrameBuffer> m_FrameBuffer;
	std::unique_ptr<Wiwa::Camera> m_Camera;

	Wiwa::Mesh *m_ActiveMesh;

	Wiwa::Vector3f m_MeshPosition;
	Wiwa::Vector3f m_MeshRotation;
	Wiwa::Vector3f m_MeshScale;

	Wiwa::Vector4f m_MeshColor;
};
