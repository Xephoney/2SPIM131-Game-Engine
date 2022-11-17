#pragma once

#include "Engine/Core.h"
#include "KeyCodes.h"

namespace Engine
{
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int keycode);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
		static void SetMousePosition(std::pair<float, float> newPos);
	};
}
