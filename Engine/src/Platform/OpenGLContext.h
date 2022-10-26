#pragma once
#include "Engine/Renderer/RenderContext.h"

struct GLFWwindow;

namespace Engine
{
	class OpenGLContext : public RenderContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		
		void init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* window;
	};
}
