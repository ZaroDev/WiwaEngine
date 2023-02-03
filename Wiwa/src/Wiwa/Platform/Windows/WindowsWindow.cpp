#include "wipch.h"
#include "WindowsWindow.h"

#include <Wiwa/utilities/Log.h>

#include "Wiwa/Events/ApplicationEvent.h"
#include "Wiwa/Events/MouseEvent.h"
#include "Wiwa/Events/KeyEvent.h"
#include "Wiwa/ImGui/ImGuiLayer.h"

#include <glew.h>
#include <gl/gl.h>

#include "../stb/stb_image.h"



namespace Wiwa {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		WI_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		m_OldData.Title = props.Title;
		m_OldData.Width = props.Width;
		m_OldData.Height = props.Height;
		m_OldData.Refresh = props.Refresh;


		WI_CORE_WARN("=======Initializing window========");
		WI_CORE_INFO("Creating window {0} {1} {2}", props.Title, props.Width, props.Height);
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			WI_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		GLenum status = glewInit();

		WI_CORE_INFO("Using Glew {0}", glewGetString(GLEW_VERSION));
		WI_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		WI_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		WI_CORE_INFO("OpenGL version supported {0}", glGetString(GL_VERSION));
		WI_CORE_INFO("GLSL: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
		WI_CORE_ASSERT(!status, "Failed to initialize Glew!");
		WI_CORE_WARN("=========Window intialized========");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		//SetVSync(true);
		//SetResizable(true);
		//Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
		glfwSetDropCallback(m_Window, [](GLFWwindow* window, int count, const char** paths) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			WindowDropEvent event(count, paths);
			data.EventCallback(event);
		});
		int w, h, ch;

		unsigned char* image = stbi_load("resources/icons/wiwa_icon.png", &w, &h, &ch, STBI_rgb_alpha);

		GLFWimage images[1];
		images[0].height = h;
		images[0].width = w;
		images[0].pixels = image;

		glfwSetWindowIcon(m_Window, 1, images);
	}
	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
	}
	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
	void WindowsWindow::SetFullScreen(bool enabled)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		m_OldData.Refresh = mode->refreshRate;
		m_Data.Refresh = mode->refreshRate;

		if(enabled)
			glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		else
			glfwSetWindowMonitor(m_Window, nullptr, (m_Data.Width / 2) - (m_OldData.Width / 2), (m_Data.Height / 2) - (m_OldData.Height / 2),
				m_OldData.Width, m_OldData.Height, m_OldData.Refresh);
		WI_CORE_INFO("Window fullscreen changed {0}, {1}, {2}", m_OldData.Width, m_OldData.Height, m_OldData.Refresh);
		m_Fullscreen = enabled;
	}
	void WindowsWindow::SetResizable(bool enabled)
	{
		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, enabled);
		m_Resizable = enabled;
	}
}
