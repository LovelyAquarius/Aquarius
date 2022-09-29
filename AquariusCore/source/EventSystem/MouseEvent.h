#pragma once

#include <sstream>
#include"EventSystem.h"


namespace Aquarius
{

	class AQUARIUS_API MouseMovedEvent : public BaseEvent
	{
	public:
		MouseMovedEvent(float x, float y) :m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
#ifdef AQ_DEBUG
			std::stringstream ss;
			ss << "MouseMovvedEvent:" << "(" << m_MouseX << "," << m_MouseY << ")";
			return ss.str();
#else
			return std::string("非Debug模式未设置，来自eventsystem。")
#endif
			
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	
	private:
		float m_MouseX, m_MouseY;

	};



	class AQUARIUS_API MouseScrolledEvent : public BaseEvent
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) :m_xOffset(xOffset), m_yOffset(yOffset) {}

		inline float GetxOffset() const { return m_xOffset; }
		inline float GetyOffset() const { return m_yOffset; }

		std::string ToString() const override
		{
#ifdef AQ_DEBUG
			std::stringstream ss;
			ss << "MouseScrolledEvent:" << "(" << m_xOffset << "," << m_yOffset << ")";
			return ss.str();
#else
			return std::string("非Debug模式未设置，来自eventsystem。")
#endif

		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_xOffset, m_yOffset;

	};


	class AQUARIUS_API MouseButtonEvent : public BaseEvent
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:

		MouseButtonEvent(int button) : m_Button(button) {}

		int m_Button;

	};

	class AQUARIUS_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
#ifdef AQ_DEBUG
			std::stringstream ss;
			ss << "MouseButtonPressedEvent:" << m_Button;
			return ss.str();
#else
			return std::string("非Debug模式未设置，来自eventsystem。")
#endif
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)

	};

	class AQUARIUS_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
#ifdef AQ_DEBUG
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent:" << m_Button;
			return ss.str();
#else
			return std::string("非Debug模式未设置，来自eventsystem。")
#endif
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)

	};




}