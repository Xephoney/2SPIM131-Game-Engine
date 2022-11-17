#pragma once

#include "engpch.h"

#include "Window.h"
#include "Engine/Events/Event.h"
#include "Engine/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Scene/Scene.h"

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
		std::shared_ptr<Scene> GetScene() const { return loadedScene; }

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlay);

		ImGuiLayer* GetImGuiLayer();

		void Close();
		
		std::shared_ptr<Scene> loadedScene;
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		[[nodiscard]] const double& CalculateDeltaTime() const ;

	private:
		
		//Private Variables
		std::unique_ptr<Window> m_Window;
		
		ImGuiLayer* m_ImGuiLayer { nullptr };
		bool m_Running = true;
		bool m_Minimized = false;
		static Application* s_instance;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}

