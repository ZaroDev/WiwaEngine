#include "../../../headers/utilities/renderer/FrameBuffer.h"
#include "../../../headers/Video.h"
#include "../../../Application.h"

void EM::FrameBuffer::Init()
{
	Video* video = Application::GetInstance()->video;
	// FRAMEBUFFER
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// Color texture
	glGenTextures(1, &m_ColorBufferTexture);
	glBindTexture(GL_TEXTURE_2D, m_ColorBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, video->getScreenWidth(), video->getScreenHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBufferTexture, 0);
	// Render buffer object in the future for depth
	//glGenRenderbuffers(1, &m_RBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, video->getScreenWidth(), video->getScreenHeight());
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Debug::LogError("Renderer2D", "Framebuffer not completed");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EM::FrameBuffer::Bind()
{
	Video* video = Application::GetInstance()->video;

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	//glViewport(0, 0, video->getScreenWidth(), video->getScreenHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EM::FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
