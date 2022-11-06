#pragma once

#include "KeyCodes.h"

namespace Engine
{
	class  Input
	{
	public:
		virtual ~Input() = default;
		inline static bool IsKeyPressed(int keycode) {	return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int keycode) { return s_Instance->IsMouseButtonPressedImpl(keycode); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static void SetMousePosition(std::pair<float, float> newPos) { s_Instance->SetMousePositionImpl(newPos);  }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int keycode) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual void SetMousePositionImpl(std::pair<float, float> newPos) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;

	private:
		static Input* s_Instance; 
	};
}
