#pragma once

#include "Engine/engpch.h"

#include "Core.h"
#include "Window.h"
#include "Engine/Events/Event.h"
#include "Engine/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

namespace Engine
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void OnEvent(Event& e);
		static Application& GetApplication() { return *s_instance; }

		Window& GetWindow() const { return *m_Window; }

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlay);

	private:
		bool OnWindowClose(WindowCloseEvent& event);

	private:
		//Private Variables
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer { nullptr };
		bool m_Running = true;
		static Application* s_instance;
		LayerStack m_LayerStack;
	};

	


	Application* CreateApplication();

}

