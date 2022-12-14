#include "engpch.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Application.h"

#include "Log.h"

#include "Renderer/Renderer.h"



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
			static double time = 0;
			const auto dt = CalculateDeltaTime();
			time += dt;

			if(!m_Minimized)
			{
				for (Layer* l : m_LayerStack)
					l->OnUpdate(dt);
			}

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
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

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

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		if(event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
		return false;
	}

	const double& Application::CalculateDeltaTime() const
	{
		static std::chrono::steady_clock::time_point lastFrame = std::chrono::high_resolution_clock::now();
		const std::chrono::steady_clock::time_point thisFrame = std::chrono::high_resolution_clock::now();

		const auto deltaTimeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(thisFrame - lastFrame).count(); //milliseconds
		const auto deltaTimeS = static_cast<double>(deltaTimeNS) /  1000000000.0;
		lastFrame = thisFrame;
		return deltaTimeS;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);		
	}

	void Application::PushOverLay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	ImGuiLayer* Application::GetImGuiLayer()
	{
		return m_ImGuiLayer;
	}

	void Application::Close()
	{
		m_Running = false;
		//exit(0);
	}
}
