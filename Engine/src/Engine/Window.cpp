
#include "engpch.h"
#include "Window.h"

#include "Log.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Platform/OpenGLContext.h"


namespace Engine
{
	static bool s_GLFWinitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ENGINE_LOG_ERROR("GLFW ERROR ({0}): {1}", error, description)
	}
	
	void Window::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Data.EventCallback = callback;
	}

	void Window::SetVSync(bool enable)
	{
		m_vsync = enable;
		if (enable)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enable;
	}

	Window* Window::Create(const WindowProperties& properties)
	{
		return new Window(properties);
	}
	
	void Window::Init(const WindowProperties& properties)
	{
		m_Data.Width = properties.Width;
		m_Data.Height = properties.Height;
		m_Data.Title = properties.WindowTitle;

		// Verify that GLFW has not been initialized yet. 
		if(s_GLFWinitialized == false)
		{
			int success = glfwInit();
			ENGINE_CORE_ASSERT(success, "Could not initialize GLFW");
			s_GLFWinitialized = true;
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		
		m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width), static_cast<int>(m_Data.Height), m_Data.Title.c_str(), nullptr, nullptr);
		m_Context = new OpenGLContext(m_Window);
		m_Context->init();
		
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		glClearColor(0.f, 0.3f, 0.4f, 1.0f);

		SetVSync(false);


		// ------------------------------ Setup events ---------------------------
		// Window Resize Event
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int Width, int Height)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				data.Width = Width;
				data.Height = Height;

				WindowResizeEvent event ( Width, Height);
				data.EventCallback(event);
			});

		// Window Close Event
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		// Key input event
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				
				switch(action)
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
				default: ;
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(character);
				data.EventCallback(event);
			});

		// MouseButton Event
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
				WindowData& a = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						a.EventCallback(event);
						break;

					}
					case GLFW_RELEASE :
					{
						MouseButtonReleasedEvent event(button);
						a.EventCallback(event);
						break;
					}

					default:
						break;
				}
		});

		// Scroll wheel Event
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x_offset, double y_offset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event(static_cast<float>(x_offset), static_cast<float>(y_offset));
			data.EventCallback(event);
			
		});

		// Cursor Moved Event
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
			data.EventCallback(event);

		});
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	Window::Window(const WindowProperties& properties)
	{
		Init(properties);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}
}
