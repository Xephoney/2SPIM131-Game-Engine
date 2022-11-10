#pragma once

#include "Engine/Input.h"

namespace Engine
{
	class WindowsInput : public Input
	{
		~WindowsInput();
	protected :
		bool IsKeyPressedImpl(int keycode) override;
		bool IsMouseButtonPressedImpl(int keycode) override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
		virtual void SetMousePositionImpl(std::pair<float, float> newPos) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;

		
	};
}