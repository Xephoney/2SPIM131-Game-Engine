
#include <Engine.h>

#include <imgui.h>

#include "Engine/Scene/Components.h"
#include "glm/ext/matrix_transform.hpp"

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
class ExampleLayer : public Engine::Layer
{
public:
	double _dt;
	double _elapsed {0};
	ExampleLayer() : Layer("MyFirstLayer")
	{
		
	}

	void OnUpdate(const double& dt) override
	{
		_dt = dt;
		_elapsed += _dt;
	}
	void OnImGuiRender() override
	{
		ImGui::Begin("Test Window | EXAMPLE LAYER");
		ImGui::Text("Deltatime = %f", _dt);

		ImGui::Text("Elapsed Time = %f", _elapsed);
		ImGui::Separator();
		ImGui::CollapsingHeader("Movement Controls");
		ImGui::Text("\t\t(while holding right-click)");
		ImGui::BulletText("WASD - Move camera");
		ImGui::BulletText("Q - Down");
		ImGui::BulletText("E - Up");
		ImGui::BulletText("Moving your mouse turns the camera");
		ImGui::End();
	}

	void OnEvent(Engine::Event& event) override
	{
		Engine::EventDispatcher dispatcher(event);
		
		ENGINE_LOG_TRACE("{0}", event.ToString())

		if(event.GetEventType() == Engine::EventType::KeyPressed)
		{
			auto& newEvent = static_cast<Engine::KeyPressedEvent&>(event);
			if(newEvent.GetKeyCode() == KEY_SPACE)
			{
				// SPACE IS BEING PRESSED <3 
			}
		}

		if(Engine::Input::IsKeyPressed(KEY_SPACE))
		{
			
		}
	}

	
};


class GameLayer : public Engine::Layer
{
	std::shared_ptr<Engine::Scene> scene;
	double elapsedTime{ 0 };
	int entity_count = 0;
public:
	GameLayer() : Layer("GameLayer")
	{
		srand(static_cast<unsigned>(time(nullptr)));
		scene = Engine::Application::GetApplication().GetScene();
	}

	void OnUpdate(const double& dt) override
	{
		elapsedTime += dt;
	}
	void OnImGuiRender() override
	{
		ImGui::Begin("Scene Viewer | GAME LAYER");
		auto& reg = scene->GetReg();
		const auto entities = scene->GetReg().view<Engine::Tag>();
		
		if (ImGui::TreeNode("Scene Graph (Version 1)"))
		{
			HelpMarker("Hold CTRL and click to select multiple items.");
			static bool selection[8192];
			
			for (int n = 0; n < static_cast<int>(entities.size()); n++)
			{
				char buffer[32];
				auto& tag = reg.get<Engine::Tag>(entities[n]).tag;
				sprintf(buffer, "%s", tag.c_str(), n);
				
				if (ImGui::Selectable(buffer, selection[n]))
				{
					if (!ImGui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
						memset(selection, 0, sizeof(selection));
					selection[n] ^= 1;
				}
			}

			ImGui::TreePop();

			ImGui::Separator();
			if (ImGui::Button("Delete"))
			{
				int size = entities.size()+1;
				for(int i = size; i >= 0; i--)
				{
					if(selection[i] == true)
					{
						selection[i] = false;
						reg.destroy(entities[i]);
						entity_count--;
					}
				}
			}
		}
		
					
		ImGui::End();
	}
	void OnEvent(Engine::Event& event) override
	{
		Engine::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FN(GameLayer::OnSpacePressed));
	}
	bool OnSpacePressed(Engine::Event& event)
	{
		if(event.GetEventType() == Engine::EventType::KeyPressed)
		{
			const auto& newEvent = dynamic_cast<Engine::KeyPressedEvent&>(event);
			if(newEvent.GetKeyCode() == KEY_TAB)
			{
				//Spawn Entity
				float x = (static_cast<float>(rand() % 16) / 2.f ) - 4.f;
				float y = (static_cast<float>(rand() % 16) / 2.f ) - 4.f;

				entity_count++;
				std::string name = "Cube ";
				name += std::to_string(entity_count);
				Engine::Entity cube = scene->CreateEntity(name);
				
				cube.GetComponent<Engine::Transform>().transform = glm::translate(glm::mat4{ 1.f }, { x,y,0 });
			}
		}
		return false;
	}
};

class SoundLayer : public Engine::Layer
{
public:
	Engine::sound gameSound;


	SoundLayer() : Layer("SoundLayer") {

	}
	void OnUpdate(const double& dt) override {
		gameSound.update();
	}
	void OnEvent(Engine::Event& event) override
	{
		Engine::EventDispatcher dispatcher(event);

		ENGINE_LOG_TRACE("{0}", event.ToString())

			if (event.GetEventType() == Engine::EventType::KeyPressed)
			{
				auto& newEvent = static_cast<Engine::KeyPressedEvent&>(event);
				if (newEvent.GetKeyCode() == KEY_SPACE)
				{
					
				}
			}

		if (Engine::Input::IsKeyPressed(KEY_SPACE))
		{

		}
	}
};

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new GameLayer());
		PushLayer(new SoundLayer());
	}

	~Sandbox()
	{
		
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}
