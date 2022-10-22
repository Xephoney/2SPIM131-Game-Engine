#pragma once

#include <Engine/engpch.h>
#include <GLFW/glfw3.h>
#include "Core.h"

#include "Engine/Events/Event.h"


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
		virtual void OnUpdate();
		uint16_t GetWidth() const { return m_Data.Width; }
		uint16_t GetHeight() const { return m_Data.Height; }

		using EventCallbackFn = std::function<void(Event&)>;
		virtual void SetEventCallback(const EventCallbackFn&);
		void SetVSync(bool enable);
		void isVSync() const;

		static Window* Create(const WindowProperties& properties = WindowProperties());



	private:
		void Init(const WindowProperties& properties);
		void Shutdown();

		GLFWwindow* m_Window{ nullptr };
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
		bool m_vsync{ false };
		bool m_initialized { false };
		EventCallbackFn EventCallback;

	};
}
