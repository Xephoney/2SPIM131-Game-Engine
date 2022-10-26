#include "engpch.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Application.h"

#include "Log.h"
#include "backends/imgui_impl_glfw.h"
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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		
		float vertices[3 * 3]
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		unsigned int indices[3] = { 0,1,2 };

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));
		m_IndexBuffer->Bind();

		std::string vertexShaderTemp = R"(

			#version 410 core
			layout(location = 0) in vec3 positionIn;

			out vec3 v_pos;

			void main()
			{
				gl_Position = vec4(positionIn,1);
				v_pos = positionIn;
			}
		)";
		std::string fragmentShaderTemp = R"(

			#version 410 core
			layout(location = 0) out vec4 fragmentColor;
			in vec3 v_pos;

			void main()
			{
				fragmentColor = vec4(v_pos+0.5, 1.0);
			}
		)";

		shader.reset(new Shader(vertexShaderTemp,fragmentShaderTemp));

	}

	Application::~Application() = default;

	void Application::run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
