#include "engpch.h"

#include "WindowsInput.h"

#include <GLFW/glfw3.h>

#include "Engine/Application.h"

namespace Engine
{
	Input* Input::s_Instance = new WindowsInput();

	WindowsInput::~WindowsInput()
	{

	}

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());

		int keyStatus = glfwGetKey(Window, keycode);
		return keyStatus == GLFW_PRESS || keyStatus == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int keycode)
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(Window, keycode);
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		// Auto here is a GLFWwindow*
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double x_pos, y_pos;
		glfwGetCursorPos(Window, &x_pos, &y_pos);

		//std::pair<float, float>
		return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
	}

	float WindowsInput::GetMouseXImpl()
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double x_pos, y_pos;
		glfwGetCursorPos(Window, &x_pos, &y_pos);
		return static_cast<float>(x_pos);
	}

	float WindowsInput::GetMouseYImpl()
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double x_pos, y_pos;
		glfwGetCursorPos(Window, &x_pos, &y_pos);
		return static_cast<float>(y_pos);
	}

	void WindowsInput::SetMousePositionImpl(std::pair<float, float> newPos)
	{
		const auto Window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		glfwSetCursorPos(Window, newPos.first, newPos.second);
	}
}
