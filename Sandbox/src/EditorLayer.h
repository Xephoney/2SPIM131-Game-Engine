#pragma once
#include "Engine/Layer.h"
#include <glm/glm.hpp>

#include "imgui.h"
#include "Engine/Application.h"
#include "Engine/Input.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/Scene.h"
//#include "Engine/Scene/SceneSerializer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "Panels/SceneHierarchyPanel.h"

#include "ImGuizmo.h"
#include "Engine/Math/Math.h"
#include "glm/gtc/type_ptr.hpp"


namespace Engine
{
	class EditorLayer : public Layer
	{
	public:
		double _dt {0};
		double _elapsed{ 0 };
		glm::vec2 renderwindowSize{ 1920, 1080 };
		glm::vec2 renderwindowCenter;
		glm::vec2 viewportBounds[2];
		glm::vec2 viewportSize{1280, 720};
		bool viewportFocused = false;
		bool viewportHovered = false;
		std::shared_ptr<Scene> activeScene;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Framebuffer> m_FrameBuffer;
		SceneHierarchyPanel m_SceneGraph;
		int m_guizmoType = 0;


		EditorLayer() : Layer("EditorLayer")
		{
			activeScene = std::make_shared<Scene>();
			Application::GetApplication().loadedScene = activeScene;

			camera = std::make_shared<PerspectiveCamera>(60.f, (16.f / 9.f), 0.01f, 1000.f);
			camera->SetPosition({ 20,17,20 });
			camera->Direction() = glm::normalize(glm::vec3(0.f) - camera->GetPosition() );
			FramebufferSpesification fbs;

			//						Color Texture	                     Mouse picking (Entity ID)						( Depth Texture ) 
			fbs.attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
			fbs.width = 1280;
			fbs.height = 720;
			fbs.samples = 1;
			m_FrameBuffer = Framebuffer::Create(fbs);


		}
		bool bMB1Pressed{ false };
		void OnUpdate(const double& dt) override
		{
			_dt = dt;
			_elapsed += _dt;
			//Frame setup
			{
				FramebufferSpesification spec = m_FrameBuffer->GetSpesification();
				if (viewportSize.x > 0.0f && viewportSize.y > 0.0f && (spec.width != static_cast<uint32_t>(viewportSize.x) || spec.height != static_cast<uint32_t>(viewportSize.y)))
				{
					const auto x = static_cast<uint32_t>(viewportSize.x);
					const auto y = static_cast<uint32_t>(viewportSize.y);
					if (x > 0 && y > 0)
					{
						m_FrameBuffer->Resize(x, y);
						camera->Resize(x, y);
						activeScene->OnViewportResize(x, y);
					}
				}
				Renderer::NewFrame(camera);
			}
			//Normal loop
			{
				if (viewportHovered)
					MoveEditorCamera();

				camera->update(dt);


				activeScene->ShadowPass();

				m_FrameBuffer->Bind();
				RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.3f, 1.f });
				RenderCommand::Clear();

				activeScene->OnUpdate(dt);

				if (Input::IsMouseButtonPressed(MOUSE_BUTTON_1) && !bMB1Pressed && viewportHovered && (!ImGuizmo::IsOver() && !ImGuizmo::IsUsing()))
				{
					bMB1Pressed = true;
					int clickedData = SampleViewportAtMouseLocation();
					if (clickedData >= 0)
						m_SceneGraph.PickEntity(static_cast<uint32_t>(clickedData));
					else
						m_SceneGraph.ClearEntity();
				}
				
			}
			//Clean up
			{
				m_FrameBuffer->Unbind();
				Renderer::RenderFrame();
			}
		}
		void OnAttach() override
		{
			m_SceneGraph.SetContext(activeScene); 
			
			/*SceneSerializer serializer(activeScene);
			serializer.SerializeText("../Engine/Assets/Scenes/EditorExample.lvl");*/

		}
		void OnImGuiRender() override
		{
			// IMGUI BOILERPLATE
			static bool p_open = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

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
						Application::GetApplication().Close();
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

			ImGui::Begin("##deltatime", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::Text("Deltatime = %f ms", _dt * 1000);
			ImGui::Text("Dynamic Rigidbodies %d", activeScene->physicsWorld->EntityCount);
			ImGui::End();

			// ------------------------------ RENDER WINDOW ------------------------------
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("Scene##Scene View");
			ImVec2 _viewportSize = ImGui::GetContentRegionAvail();
			viewportSize.x = _viewportSize.x;
			viewportSize.y = _viewportSize.y;
			auto viewportOffset = ImGui::GetCursorPos(); //ImVec2

			viewportFocused = ImGui::IsWindowFocused();
			viewportHovered = ImGui::IsWindowHovered();
			if (viewportFocused)
			{
				renderwindowCenter.x = ImGui::GetWindowViewport()->GetCenter().x;
				renderwindowCenter.y = ImGui::GetWindowViewport()->GetCenter().y;
			}
			Application::GetApplication().GetImGuiLayer()->SetSkipEvent(viewportFocused || viewportHovered);
			//Application::GetApplication().GetImGuiLayer()->SetBlockEvents(!viewportFocused || !viewportHovered);

			uint32_t colorTextureID = m_FrameBuffer->GetColorAttachment();
			ImGui::Image(reinterpret_cast<void*>(colorTextureID), _viewportSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 minBound = ImGui::GetWindowPos();
			// minBound.x += viewportOffset.x;
			// minBound.y += viewportOffset.y;
			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			viewportBounds[0] = { minBound.x, minBound.y };
			viewportBounds[1] = { maxBound.x, maxBound.y };

			// RENDER WINDOW GIZMO
			Entity selectedEntity = m_SceneGraph.GetSelectedEntity();
			if(selectedEntity && m_guizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);
				const glm::mat4& view = camera->GetViewMatrix();
				const glm::mat4& projection = camera->GetProjectionMatrix();

				auto& tc = selectedEntity.GetComponent<Transform>();
				glm::mat4& transform = tc.transform;

				// Snapping
				bool snap = Input::IsKeyPressed(KEY_LEFT_CONTROL);
				float snapValue = 0.1f; // Snap to 0.5m for translation/scale
				// Snap to 10 degrees for rotation
				if (m_guizmoType == ImGuizmo::OPERATION::SCALE)
					snapValue = 0.1f;
				if (m_guizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 11.25f;

				float snapValues[3] = { snapValue, snapValue, snapValue };



				ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
					(ImGuizmo::OPERATION)m_guizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				
				if(ImGuizmo::IsUsing())
				{
					glm::vec3 position, rotation, scale;
					Math::DecomposeTransform(transform, position, rotation, scale);
					const glm::vec3 deltaRot = rotation - tc.euler_rotation;
		
					tc.position = position;
					tc.Rotate(deltaRot);
					tc.scale = scale;
				}
			}



			ImGui::End();

			ImGui::PopStyleVar();

			m_SceneGraph.OnImGuiRender();

			ImGui::Begin("##Toolbar",nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			if(!activeScene->IsSimulating())
			{
				if(ImGui::Button("Start Simulation"))
				{
					activeScene->StartSimulation();
				}
				
			}
			else
			{
				if (ImGui::Button("Pause Simulation"))
				{
					activeScene->StopSimulation();
				}
			}
			ImGui::End();
			ImGui::SameLine();
		}

		void OnEvent(Event& event) override
		{
			EventDispatcher dispatcher(event);

			ENGINE_LOG_TRACE("{0}", event.ToString())

				if (event.GetEventType() == EventType::KeyPressed && !Input::IsMouseButtonPressed(MOUSE_BUTTON_2))
				{
					const auto& newEvent = dynamic_cast<KeyPressedEvent&>(event);

					switch (newEvent.GetKeyCode())
					{
					case KEY_Q :
						m_guizmoType = -1;
						break;
					case KEY_W :
						m_guizmoType = ImGuizmo::OPERATION::TRANSLATE;
						break;
					case KEY_E :
						m_guizmoType = ImGuizmo::OPERATION::ROTATE;
						break;
					case KEY_R:
						m_guizmoType = ImGuizmo::OPERATION::SCALE;
						break;
					}
					if (newEvent.GetKeyCode() == KEY_SPACE)
					{
						
					}
				}

			if (event.GetEventType() == EventType::MouseButtonReleased)
			{
				const auto& newEvent = dynamic_cast<MouseButtonReleasedEvent&> (event);
				if (newEvent.GetMouseButton() == MOUSE_BUTTON_1)
					bMB1Pressed = false;
			}

			if (event.GetEventType() == EventType::MouseScrolled && Input::IsMouseButtonPressed(MOUSE_BUTTON_2))
			{
				const auto& newEvent = dynamic_cast<MouseScrolledEvent&>(event);
				camera->m_movementSpeed += newEvent.GetYOffset() * 0.5f;
			}


		}

		void MoveEditorCamera()
		{
			glm::vec3 movement_direction{ 0,0,0 };
			if (Input::IsKeyPressed(KEY_W))
			{
				movement_direction += camera->Forward();
			}
			if (Input::IsKeyPressed(KEY_A))
			{
				movement_direction -= camera->Right();
			}
			if (Input::IsKeyPressed(KEY_S))
			{
				movement_direction -= camera->Forward();
			}
			if (Input::IsKeyPressed(KEY_D))
			{
				movement_direction += camera->Right();
			}
			if (Input::IsKeyPressed(KEY_Q))
			{
				movement_direction -= camera->Up();
			}
			if (Input::IsKeyPressed(KEY_E))
			{
				movement_direction += camera->Up();
			}


			static bool bFirstClick = true;
			static glm::vec2 center{ renderwindowSize.x / 2.f, renderwindowSize.y / 2.f };
			glm::vec2 mousePos;
			glm::vec2 delta;
			const float cameraSensitivity = 55.f;
			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_2))
			{
				//center = renderwindowCenter;
				if (!bFirstClick)
				{
					auto [x, y] = Input::GetMousePosition();
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
				Input::SetMousePosition({ center.x, center.y });
			}
			else
				bFirstClick = true;
		}

		int SampleViewportAtMouseLocation() const
		{
			//Check mouse input

			auto [mx, my] = ImGui::GetMousePos();
			mx -= viewportBounds[0].x;
			my -= viewportBounds[0].y;
			glm::vec2 _viewportSize = viewportBounds[1] - viewportBounds[0];

			my = _viewportSize.y - my;
			int moX = (int)mx;
			int moY = (int)my;

			if (moX >= 0 && moY >= 0 && moX < (int)_viewportSize.x && moY < (int)_viewportSize.y)
			{
				int data = m_FrameBuffer->ReadPixel(1, moX, moY);
				ENGINE_LOG_INFO("Pixel Data at [{0},{1}] = {2}", moX, moY, data)
					return data;
			}

			return -10;
		}
	};
}
