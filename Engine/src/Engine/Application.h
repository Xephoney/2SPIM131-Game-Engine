#pragma once

#include "Engine/engpch.h"

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void OnEvent(Event& e);
		inline static Application& GetApplication() { return *s_instance; }

		inline Window& GetWindow() { return *m_Window; }

		//Private Functions/Methods
	private:
		
		bool OnWindowCloseEvent(WindowCloseEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);

		// Type in character codes
		bool OnKeyTypedEvent(KeyTypedEvent& e);

		//Private Variables
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		static Application* s_instance;
	};

	

	Application* CreateApplication();

}

