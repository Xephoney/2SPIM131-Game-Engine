#pragma once

// TODO (PCH) : Include precompiled header with <string>
#include <string>
#include <GLFW/glfw3.h>
#include "Core.h"


namespace Engine
{
	struct WindowProperties
	{
		std::string	WindowTitle;
		uint16_t Width;
		uint16_t Height;

		WindowProperties
		(const std::string& title = "Game Engine Architecture 2022",
			uint16_t width = 1280,
			uint16_t height = 720)
				: WindowTitle(title), Width(width), Height(height)
		{
		}
	};

	class ENGINE_API Window
	{
	public:
		Window(const WindowProperties& properties);
		virtual ~Window() {}

		//TODO (Window Abstraction) : Make virtual
		void OnUpdate();
		uint16_t GetWidth() const { return m_Data.Width; }
		uint16_t GetHeight() const { return m_Data.Height; }

		//TODO (EVENTS): Add function after implementing Event system 
		//virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		void SetVSync(bool enable);
		void isVSync() const;

		static Window* Create(const WindowProperties& properties = WindowProperties());
		

	private:
		void Init(const WindowProperties& properties);
		void Shutdown();

		GLFWwindow* m_Window{ nullptr };
		WindowProperties m_Data;
		bool m_vsync{ false };
		bool m_initialized { false };
	};
}
