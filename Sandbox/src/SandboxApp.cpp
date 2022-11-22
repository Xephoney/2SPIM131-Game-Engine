
#include <Engine.h>

#include <imgui.h>

#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/Components.h"

#include <glm/glm.hpp>

#include "Engine/Scripting/Scripting.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

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
class EditorLayer : public Engine::Layer
{
public:
	double _dt;
	double _elapsed {0};
	glm::vec2 renderwindowSize;
	glm::vec2 renderwindowCenter;
	bool viewportFocused = false;
	bool viewportHovered = false;
	std::shared_ptr<Engine::Scene> activeScene;
	std::shared_ptr<Engine::Camera> camera;
	std::shared_ptr<Engine::Framebuffer> m_FrameBuffer;


	EditorLayer() : Layer("MyFirstLayer")
	{
		activeScene = std::make_shared<Engine::Scene>();
		Engine::Application::GetApplication().loadedScene = activeScene;
		
		camera = std::make_shared<Engine::PerspectiveCamera>(60.f, (16.f / 9.f), 0.01f, 1000.f);

		Engine::FramebufferSpesification fbs;
		//						Vanlig Farge texture                       Mouse picking (Entity ID)             ( Depth Texture ) 
		fbs.attachments = { Engine::FramebufferTextureFormat::RGBA8, Engine::FramebufferTextureFormat::RGBA8, Engine::FramebufferTextureFormat::Depth };
		fbs.width = 1280;
		fbs.height = 720;
		fbs.samples = 1;
		m_FrameBuffer = Engine::Framebuffer::Create(fbs);
		

	}

	void OnUpdate(const double& dt) override
	{
		_dt = dt;
		_elapsed += _dt;
		//Frame setup
		{
			Engine::Renderer::NewFrame(*camera);
			m_FrameBuffer->Bind();
			Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			Engine::RenderCommand::Clear();
		}

		//Normal loop
		{
			if(viewportFocused)
				MoveEditorCamera();

			camera->update(dt);
			activeScene->OnUpdate(dt);
		}

		//Clean up
		{
			m_FrameBuffer->Unbind();
			Engine::Renderer::RenderFrame();
		}
		
	}
	void OnImGuiRender() override
	{

		static bool p_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					Engine::Application::GetApplication().Close();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					p_open = false;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		ImGui::End();

		ImGui::Begin("Test Window | EXAMPLE LAYER");
		ImGui::Text("Deltatime = %f ms", _dt*1000);

		ImGui::Text("Elapsed Time = %f", _elapsed);
		ImGui::Separator();
		ImGui::CollapsingHeader("Movement Controls");
		ImGui::Text("\t\t(while holding right-click)");
		ImGui::BulletText("WASD - Move camera");
		ImGui::BulletText("Q - Down");
		ImGui::BulletText("E - Up");
		ImGui::BulletText("Moving your mouse turns the camera");
		ImGui::End();



		// ------------------------------ RENDER WINDOW ------------------------------
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Scene View");
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		viewportFocused = ImGui::IsWindowFocused();
		viewportHovered = ImGui::IsWindowHovered();
		if (viewportFocused)
		{
			renderwindowCenter.x = ImGui::GetWindowViewport()->GetCenter().x;
			renderwindowCenter.y = ImGui::GetWindowViewport()->GetCenter().y;
		}
		Engine::Application::GetApplication().GetImGuiLayer()->SetSkipEvent(viewportFocused || viewportHovered);
		//Engine::Application::GetApplication().GetImGuiLayer()->SetBlockEvents(!viewportFocused || !viewportHovered);
		if(static_cast<int>(viewportSize.x) != static_cast<int>(renderwindowSize.x) 
			|| static_cast<int>(viewportSize.y) != static_cast<int>(renderwindowSize.y))
		{
			renderwindowSize = { viewportSize.x, viewportSize.y };
			m_FrameBuffer->Resize(renderwindowSize.x, renderwindowSize.y);
			camera->Resize(renderwindowSize.x, renderwindowSize.y);
		}
		uint32_t textureID = m_FrameBuffer->GetColorAttachment();
		ImGui::Image(reinterpret_cast<void*>(textureID), viewportSize,ImVec2{0,1}, ImVec2{ 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void OnEvent(Engine::Event& event) override
	{
		Engine::EventDispatcher dispatcher(event);
		
		ENGINE_LOG_TRACE("{0}", event.ToString())

		if(event.GetEventType() == Engine::EventType::KeyPressed)
		{
			const auto& newEvent = dynamic_cast<Engine::KeyPressedEvent&>(event);
			if(newEvent.GetKeyCode() == KEY_SPACE)
			{
				// SPACE IS BEING PRESSED <3 
			}
		}

		if (event.GetEventType() == Engine::EventType::MouseScrolled && Engine::Input::IsMouseButtonPressed(MOUSE_BUTTON_2))
		{
			const auto& newEvent = dynamic_cast<Engine::MouseScrolledEvent&>(event);
			camera->m_movementSpeed += newEvent.GetYOffset() * 0.5f;
		}
	}

	void MoveEditorCamera()
	{
		glm::vec3 movement_direction{ 0,0,0 };
		if (Engine::Input::IsKeyPressed(KEY_W))
		{
			movement_direction += camera->Forward();
		}
		if (Engine::Input::IsKeyPressed(KEY_A))
		{
			movement_direction -= camera->Right();
		}
		if (Engine::Input::IsKeyPressed(KEY_S))
		{
			movement_direction -= camera->Forward();
		}
		if (Engine::Input::IsKeyPressed(KEY_D))
		{
			movement_direction += camera->Right();
		}
		if (Engine::Input::IsKeyPressed(KEY_Q))
		{
			movement_direction -= camera->Up();
		}
		if (Engine::Input::IsKeyPressed(KEY_E))
		{
			movement_direction += camera->Up();
		}
		

		static bool bFirstClick = true;
		static glm::vec2 center{ renderwindowSize.x / 2.f, renderwindowSize.y / 2.f };
		glm::vec2 mousePos;
		glm::vec2 delta;
		const float cameraSensitivity = 55.f;
		if (Engine::Input::IsMouseButtonPressed(MOUSE_BUTTON_2))
		{
			//center = renderwindowCenter;
			if (!bFirstClick)
			{
				auto [x, y] = Engine::Input::GetMousePosition();
				mousePos.x = x;
				mousePos.y = y;
				delta = mousePos - center;

				glm::vec3 newDir = glm::rotate(camera->Direction(), glm::radians(-delta.y * cameraSensitivity * static_cast<float>(_dt)), camera->Right());
				if (!(glm::angle(newDir, { 0,1,0 }) <= glm::radians(5.f) || glm::angle(newDir, { 0,-1,0 }) <= glm::radians(5.f)))
				{
					camera->Direction() = glm::normalize(newDir);
				}

				camera->Direction() = glm::normalize(glm::rotate(camera->Direction(), glm::radians(-delta.x * cameraSensitivity * static_cast<float>(_dt)), { 0, 1, 0 }));
				if (glm::length(movement_direction) > 0.01f)
				{
					movement_direction = glm::normalize(movement_direction);
					camera->m_movementDir = movement_direction;
				}
			}

			bFirstClick = false;
			Engine::Input::SetMousePosition({ center.x, center.y });
		}
		else
			bFirstClick = true;
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
		Engine::PythonScript pyscript;

	}

	void OnUpdate(const double& dt) override
	{
		elapsedTime += dt;
	}
	void OnImGuiRender() override
	{
		if (scene)
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
							reg.destroy(entities[i]);
							entity_count--;
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete All"))
				{
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
		if(event.GetEventType() == Engine::EventType::KeyPressed)
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
				
				cube.GetComponent<Engine::Transform>().transform = glm::translate(glm::mat4{ 1.f }, { x,y,0 });
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
					gameSound.testSound();
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
		PushLayer(new EditorLayer());
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
