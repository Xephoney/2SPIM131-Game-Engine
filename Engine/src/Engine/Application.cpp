#include "Engine/engpch.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Application.h"

#include "Log.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Input.h"
#include "Engine/KeyCodes.h"
#include "Engine/MouseButtonCodes.h"

#include "glad/glad.h"



namespace Engine
{

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		
		ENGINE_CORE_ASSERT(!s_instance, "An instance of Application already exists")
		s_instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);
	}

	Application::~Application() = default;

	void Application::run()
	{
		while (m_Running)
		{
			glClearColor(0.5f, 0.2f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* l : m_LayerStack)
				l->OnUpdate();

			//Run IMGUI Rendering on all sub layers
			m_ImGuiLayer->Begin();
			for (Layer* l : m_LayerStack)
				l->OnImGuiRender();
			m_ImGuiLayer->End();
			
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);		
	}

	void Application::PushOverLay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
}
