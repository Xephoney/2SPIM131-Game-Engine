#include <engpch.h>
#include "OpenGLContext.h"
#include "Engine/Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace Engine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: window(windowHandle)
	{
		
	}

	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		ENGINE_CORE_ASSERT(status, "Failed to initialize GLAD");

		ENGINE_LOG_INFO("OpenGL Info:");
		ENGINE_LOG_INFO("\tVendor: \t {0}", glGetString(GL_VENDOR))
		ENGINE_LOG_INFO("\tRenderer:\t {0}", glGetString(GL_RENDERER))
		ENGINE_LOG_INFO("\tVersion:\t {0}\n------------------------------------------------------------------------------", glGetString(GL_VERSION))
	}

	void OpenGLContext::SwapBuffers()
	{
		//glfwPollEvents();
		glfwSwapBuffers(window);
	}
}
