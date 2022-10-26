#pragma once

#include <Engine/engpch.h>
#include "Engine/Renderer/RenderContext.h"
#include "Core.h"

#include <GLFW/glfw3.h>
#include "Engine/Events/Event.h"


namespace Engine
{
	struct WindowProperties
	{
		std::string	WindowTitle;
		unsigned int Width;
		unsigned int Height;

		WindowProperties
		(const std::string& title = "Game Engine Architecture 2022",
			unsigned int width = 1280,
			unsigned int height = 720)
				: WindowTitle(title), Width(width), Height(height)
		{
		}
	};

	class  Window
	{
	public:
		Window(const WindowProperties& properties);
		virtual ~Window() {}

		//TODO (Window Abstraction) : Make virtual
		virtual void OnUpdate();
		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }

		using EventCallbackFn = std::function<void(Event&)>;
		virtual void SetEventCallback(const EventCallbackFn&);
		void SetVSync(bool enable);
		void isVSync() const;

		static Window* Create(const WindowProperties& properties = WindowProperties());

		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		void Init(const WindowProperties& properties);
		void Shutdown();

		GLFWwindow* m_Window{ nullptr };
		RenderContext* m_Context;
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
