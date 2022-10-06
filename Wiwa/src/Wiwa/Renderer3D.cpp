#include <wipch.h>
#include "Renderer3D.h"

#include <Wiwa/Application.h>

#include <Wiwa/utilities/render/InstanceRenderer.h>
#include <Wiwa/Log.h>

#include <glew.h>

namespace Wiwa {
	Renderer3D::Renderer3D() {
		//IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	}

	Renderer3D::~Renderer3D()
	{
	}

	bool Renderer3D::Init()
	{
		Window& window = Application::Get().GetWindow();
		Size2i& resolution = Application::Get().GetTargetResolution();

		// Init framebuffer
		m_FrameBuffer.Init(resolution.w, resolution.h);

		// Init orthographic projection
		//m_OrthoProj = glm::ortho(0.0f, window.GetWidth(), window.GetHeight(), 0.0f, 0.1f, 100.0f);
		
		// Init main camera view
		//m_View = glm::mat4(1.0f);
		//m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, -3.0f));

		m_ActiveCamera.SetPerspective(45.0f, resolution.w / (float)resolution.h);

		WI_CORE_INFO("Renderer3D initialized");
		return true;
	}

	void Renderer3D::Update()
	{
		
	}

	void Renderer3D::Close()
	{
		
	}
}