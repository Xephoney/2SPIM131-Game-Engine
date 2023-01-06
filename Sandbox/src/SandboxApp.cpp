
#include <Engine.h>

#include <imgui.h>

#include "Engine/Scene/Components.h"
#include "EditorLayer.h"

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
		if (!scene)
		{
			ImGui::Begin("Scene Viewer | GAME LAYER");
		
			entt::registry& reg = scene->GetReg();
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
							reg.get<Engine::AudioSource>(entities[i]).swapSound("Delete");
							reg.get<Engine::AudioSource>(entities[i]).playSound();
							reg.destroy(entities[i]);
							entity_count--;
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete All"))
				{
					Engine::SoundManager::getSoundManager().playSound("DeleteAllID:0", true);
					memset(selection, 0, sizeof(selection));
					for (auto entity : entities)
					{
						reg.destroy(entity);
						entity_count--;
					}

				}
			}

			ImGui::End();
		}
	}
	void OnEvent(Engine::Event& event) override
	{
		Engine::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FN(GameLayer::OnSpacePressed));
	}
	bool OnSpacePressed(Engine::Event& event)
	{
		/*if(event.GetEventType() == Engine::EventType::KeyPressed)
		{
			const auto& newEvent = dynamic_cast<Engine::KeyPressedEvent&>(event);
			if(newEvent.GetKeyCode() == KEY_TAB)
			{
				//Spawn Entity
				float x = (static_cast<float>(rand() % 16) / 2.f ) - 4.f;
				float y = (static_cast<float>(rand() % 16) / 2.f ) - 4.f;

				entity_count++;
				std::string name = "Quad ";
				name += std::to_string(entity_count);
				Engine::Entity cube = scene->CreateEntity(name);
				
				//thing.addSound("dummy", "Delete");
				auto& thing = cube.AddComponent<Engine::AudioSource>("Pop", glm::vec3(x,y,0)); // test after!!
				cube.GetComponent<Engine::Transform>().position = { x, y,0 };
				cube.GetComponent<Engine::AudioSource>().playSound();
			}
		}
		if (event.GetEventType() == Engine::EventType::KeyPressed)
		{
			const auto& newEvent = dynamic_cast<Engine::KeyPressedEvent&>(event);
			if (newEvent.GetKeyCode() == KEY_ENTER)
			{
				//Spawn Entity
				entity_count++;
				std::string name = "Cube ";
				name += std::to_string(entity_count);
				Engine::Entity cube = scene->CreateEntity(name);
			}
		}*/
		return false;
	}
};

class SoundLayer : public Engine::Layer
{
public:


	SoundLayer() : Layer("SoundLayer") {
		
	}
	void OnUpdate(const double& dt) override {
		Engine::SoundManager::getSoundManager().update();
	}
	void OnEvent(Engine::Event& event) override
	{
		Engine::EventDispatcher dispatcher(event);

		ENGINE_LOG_TRACE("{0}", event.ToString())

			if (event.GetEventType() == Engine::EventType::KeyPressed)
			{
				auto& newEvent = static_cast<Engine::KeyPressedEvent&>(event); // Old way to test if sound works
				if (newEvent.GetKeyCode() == KEY_SPACE)
				{
					//Engine::SoundManager::getSoundManager().playSound("Trekant");
				}
				if (newEvent.GetKeyCode() == KEY_TAB)
				{
					Engine::SoundManager::getSoundManager().playSound("Hit", true);
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
		PushLayer(new Engine::EditorLayer());
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
