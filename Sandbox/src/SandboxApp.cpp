
#include <Engine.h>
/*
#include <imgui.h>
*/

class ExampleLayer : public Engine::Layer
{
public:
	Engine::sound sound;
	ExampleLayer() : Layer("MyFirstLayer")
	{
		
	}

	void OnUpdate() override
	{
		sound.system->update();
	}
	void OnImGuiRender() override
	{
		/*ImGui::Begin("Test Window | EXAMPLE LAYER");
		ImGui::Text("Heisann Sveisann fra Eksempel layer");
		ImGui::End();*/
	}

	void OnEvent(Engine::Event& event) override
	{
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FN(ExampleLayer::OnSpacePressed));
		//ENGINE_LOG_WARNING("{0}", event.ToString())
	}

	bool OnSpacePressed(Engine::Event& event)
	{
		if (event.GetEventType() == Engine::EventType::KeyPressed) {
			Engine::KeyPressedEvent newEvent = (Engine::KeyPressedEvent&)event;
			if (newEvent.GetKeyCode() == KEY_SPACE) {
				// Space
				sound.testSound();
			}
		}
		return false;
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
