
#include <Engine.h>
#include <imgui/imgui.h>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer() : Layer("MyFirstLayer")
	{
		
	}

	void OnUpdate() override
	{

	}
	void OnImGuiRender() override
	{
		/*ImGui::Begin("Test Window | EXAMPLE LAYER");
		ImGui::Text("Heisann Sveisann fra Eksempel layer");
		ImGui::End();*/
	}

	void OnEvent(Engine::Event& event) override
	{
		//LOG_INFO("{0}", event.ToString())
	}
};

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
		
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}
