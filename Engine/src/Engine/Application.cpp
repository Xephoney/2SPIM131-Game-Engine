#include "engpch.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Application.h"

#include "Log.h"

#include "Renderer/Renderer.h"

#define SIZE 16.f


namespace Engine
{

	Application* Application::s_instance = nullptr;

	Application::Application() : camera((-SIZE/2.f)*(16.f/9.f), (SIZE/2.f)*(16.f/9.f), -(SIZE / 2.f), (SIZE / 2.f))
	{
		
		ENGINE_CORE_ASSERT(!s_instance, "An instance of Application already exists")
		s_instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);

// 		
//
// 		float vertices[7 * 3]
// 		{
// 			-0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.8f, 1.f,
// 			 0.5f, -0.5f, 0.0f, 0.9f, 0.0f, 0.3f, 1.f,
// 			 0.0f,  0.5f, 0.0f, 0.1f, 0.8f, 0.1f, 1.f,
// 		};
// 		
// 		m_VertexArray.reset(VertexArray::Create());
// 		std::shared_ptr<VertexBuffer> vertexBuffer;
// 		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
//
// 		BufferLayout layout = {
// 			{ShaderDataType::Float3, "a_Position"},
// 			{ShaderDataType::Float4, "a_Color"}
// 		};
//
//
// 		vertexBuffer->SetLayout(layout);
// 		m_VertexArray->AddVertexBuffer(vertexBuffer);
//
// 		unsigned int indices[3] = { 0,1,2 };
// 		std::shared_ptr<IndexBuffer> indexBuffer;
// 		indexBuffer.reset(IndexBuffer::Create(indices, 3));
// 		m_VertexArray->SetIndexBuffer(indexBuffer);
// 				
// 		float q_vertices[7 * 4]
// 		{
// 			-0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.8f, 1.f,
// 			 0.5f, -0.5f, 0.0f, 0.9f, 0.0f, 0.3f, 1.f,
// 			 0.5f,  0.5f, 0.0f, 0.1f, 0.8f, 0.1f, 1.f,
// 			-0.5f,  0.5f, 0.0f, 0.0f, 0.2f, 0.8f, 1.f
// 		};
// 		std::shared_ptr<VertexBuffer> quadBuffer;
// 		quadBuffer.reset( VertexBuffer::Create(q_vertices, sizeof(q_vertices)));
// 		unsigned int indx[3 * 2]
// 		{ 0, 1, 2, 2, 3, 0 };
// 		std::shared_ptr<IndexBuffer> quad_i_Buffer;
// 		quad_i_Buffer.reset(IndexBuffer::Create(indx, 6));
//
// 		m_SquareVertexArray.reset(VertexArray::Create());
// 		
// 		quadBuffer->SetLayout(layout);
// 		m_SquareVertexArray->AddVertexBuffer(quadBuffer);
// 		m_SquareVertexArray->SetIndexBuffer(quad_i_Buffer);
//
//
// 		std::string vertexShaderTemp = R"(
//
// 			#version 410 core
// 			layout(location = 0) in vec3 a_Position;
// 			layout(location = 1) in vec4 a_Color;
//
// 			uniform mat4 view_projection;
// 			uniform mat4 model;
//
// 			out vec3 v_pos;
// 			out vec4 v_color;
// 			void main()
// 			{
// 				gl_Position = view_projection * model * vec4(a_Position,1);
// 				v_pos = a_Position;
// 				v_color = a_Color;
// 			}
// 		)";
// 		std::string fragmentShaderTemp = R"(
//
// 			#version 410 core
// 			layout(location = 0) out vec4 fragmentColor;
// 			in vec3 v_pos;
// 			in vec4 v_color;
//
// 			void main()
// 			{
// 				fragmentColor = v_color;
// 			}
// 		)";
//
// 		shader.reset(new Shader(vertexShaderTemp,fragmentShaderTemp));

	}

	Application::~Application() = default;

	void Application::run()
	{
		while (m_Running)
		{
			static double time = 0;
			const auto dt = CalculateDeltaTime();
			time += dt;

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			RenderCommand::Clear();

			Renderer::NewFrame(camera);
			
			camera.SetPosition({sin(time) * 2, cos(time) * 2, 0});
			// auto rot = camera.GetRotation();
			// //camera.SetRotation( rot += 0.0002f);
			//
			// Renderer::Submit(*shader, m_SquareVertexArray);
			// Renderer::Submit(*shader, m_VertexArray);
						
			for (Layer* l : m_LayerStack)
				l->OnUpdate(dt);
			
			//Run IMGUI Rendering on all sub layers
			m_ImGuiLayer->Begin();
			for (Layer* l : m_LayerStack)
				l->OnImGuiRender();
			m_ImGuiLayer->End();

			Renderer::RenderFrame();
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

	double Application::CalculateDeltaTime() const
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
}
