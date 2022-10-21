#pragma once

#include "Event.h"

#include <sstream>

namespace Engine
{
	class ENGINE_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint16_t width, uint16_t height)
			: m_Width(width), m_Height(height) {}

		uint16_t GetWidth() const { return m_Width; }
		uint16_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: (" << m_Width << ", " <<m_Height << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		uint16_t m_Width, m_Height;
	};

	class ENGINE_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
