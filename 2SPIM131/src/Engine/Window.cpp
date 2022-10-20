#include "Window.h"

namespace Engine
{
	static bool s_GLFWinitialized = false;


	void Window::SetVSync(bool enable)
	{
		m_vsync = enable;
		if (enable)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	Window* Window::Create(const WindowProperties& properties)
	{
		return new Window(properties);
	}

	void Window::Init(const WindowProperties& properties)
	{
		m_Data.Width = properties.Width;
		m_Data.Height = properties.Height;
		m_Data.WindowTitle = properties.WindowTitle;

		// Verify that GLFW has not been verified yet, and stop it if it has. 
		if(s_GLFWinitialized == false)
		{
			int success = glfwInit();
			s_GLFWinitialized = true;
		}

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.WindowTitle.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(false);
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
		glfwSwapBuffers(m_Window);
	}
}
