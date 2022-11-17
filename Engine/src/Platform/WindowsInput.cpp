#include <engpch.h>

#include <GLFW/glfw3.h>

#include "Engine/Application.h"
#include "Engine/Input.h"

namespace Engine
{
	auto Input::IsKeyPressed(int keycode) -> bool
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());

		int keyStatus = glfwGetKey(Window, keycode);
		return keyStatus == GLFW_PRESS || keyStatus == GLFW_REPEAT;
	}

	auto Input::IsMouseButtonPressed(int keycode) -> bool
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(Window, keycode);
	}

	auto Input::GetMousePosition() -> std::pair<float, float>
	{
		// Auto here is a GLFWwindow*
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double x_pos, y_pos;
		glfwGetCursorPos(Window, &x_pos, &y_pos);

		//std::pair<float, float>
		return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
	}

	auto Input::GetMouseX() -> float
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double x_pos, y_pos;
		glfwGetCursorPos(Window, &x_pos, &y_pos);
		return static_cast<float>(x_pos);
	}

	auto Input::GetMouseY() -> float
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double x_pos, y_pos;
		glfwGetCursorPos(Window, &x_pos, &y_pos);
		return static_cast<float>(y_pos);
	}

	auto Input::SetMousePosition(std::pair<float, float> newPos) -> void
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		glfwSetCursorPos(Window, newPos.first, newPos.second);
	}
}
