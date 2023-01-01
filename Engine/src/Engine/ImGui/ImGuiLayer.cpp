#include "engpch.h"

#include "ImGuiLayer.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "imgui.h"
#include "Engine/Application.h"

#include <ImGuizmo.h>

namespace Engine
{
	ImGuiLayer::ImGuiLayer()
		: Layer("IMGUI")
	{
	}
	ImGuiLayer::~ImGuiLayer() = default;

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Get keyboard?
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Aktivere Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Aktivere Viewports

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags % ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.f;
			// Bakgrunns-alpha til IMGUI Vindu. Hvor gjennomsiktig den skal være, 1 = Helt tett, 0 = helt gjennomsiktig
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::GetApplication();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnImGuiRender()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetApplication();
		io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnUpdate(const double& dt)
	{
		Layer::OnUpdate(dt);
		m_Time += dt;

	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		Layer::OnEvent(event);
		if (m_SkipEvents)
			return;

		ImGuiIO& io = ImGui::GetIO();
		event.m_Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		event.m_Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}
}
