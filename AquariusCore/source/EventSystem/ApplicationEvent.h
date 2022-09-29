#pragma once


#include "EventSystem.h"

namespace Aquarius
{
	class AQUARIUS_API WindowResizeEvent : public BaseEvent
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
#ifdef AQ_DEBUG
			std::stringstream ss;
			ss << "WindowResizeEvent:" << "(" << m_Width << "," << m_Height << ")";
			return ss.str();
#else
			return std::string("非Debug模式未设置，来自eventsystem。")
#endif
		}
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class AQUARIUS_API WindowCloseEvent : public BaseEvent
	{
	public:
			WindowCloseEvent() {}

			EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class  AppUpdateEvent : public BaseEvent
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class  AppTickEvent : public BaseEvent
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class  AppRenderEvent : public BaseEvent
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	};
}

