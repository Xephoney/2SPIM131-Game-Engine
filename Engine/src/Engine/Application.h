#pragma once

#include "engpch.h"

#include "Window.h"
#include "Engine/Events/Event.h"
#include "Engine/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Buffer.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

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

		double CalculateDeltaTime() const ;

	private:


		//Private Variables
		std::unique_ptr<Window> m_Window;
		OrthographicCamera camera; 
		ImGuiLayer* m_ImGuiLayer { nullptr };
		bool m_Running = true;
		static Application* s_instance;
		LayerStack m_LayerStack;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Shader> shader;

		std::shared_ptr<VertexArray> m_SquareVertexArray;

		
	};

	Application* CreateApplication();
}

