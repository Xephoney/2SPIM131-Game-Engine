#pragma once

#include "Engine/engpch.h"

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void OnEvent(Event& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool OnWindowClose(WindowCloseEvent& e);

		bool m_Running = true;
	};

	Application* CreateApplication();

}

