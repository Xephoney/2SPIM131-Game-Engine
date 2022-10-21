#include "Application.h"
#include <cstdio>


Engine::Application::Application()
{
	m_Window = std::unique_ptr<Window>(Window::Create());
}

Engine::Application::~Application()
{

}

void Engine::Application::run()
{
	/*
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280,
		720,
		"Game Engine Architecture 2022", nullptr, nullptr);
	
	if(!window)
	{
		printf("Failed to create GLFW Window. \n--------------------------------------------------\n        [Exiting Application]\n--------------------------------------------------\n\n\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, 1280, 720);

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	*/

	/*while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}*/
	while (m_Running)
	{
		m_Window->OnUpdate();
	}

	/*glfwDestroyWindow(window);
	glfwTerminate();*/
	
}
