#pragma once

#include "EventSystem.h"

namespace Aquarius
{

	class AQUARIUS_API KeyEvent : public BaseEvent
	{
	public:
		virtual inline  int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode) : m_KeyCode(keycode) {}

		int m_KeyCode;
	};


	class AQUARIUS_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatcount) :KeyEvent(keycode), m_RepeatCount(repeatcount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
#ifdef AQ_DEBUG
			std::stringstream ss;
			ss << "KeyPresssedEvent:" << m_KeyCode << "(" << m_RepeatCount << "repeats)";
			return ss.str();
#else
			return std::string("非Debug模式未设置，来自eventsystem。")
#endif
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};


	class  KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
#ifdef AQ_DEBUG
			std::stringstream ss;
			ss << "KeyReleasedEvent:" << m_KeyCode;
			return ss.str();
#else
			return std::string("非Debug模式未设置，来自eventsystem。");
#endif
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};


	class  KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode) :KeyEvent(keycode) {}

		std::string ToString() const override
		{
#ifdef AQ_DEBUG
			std::stringstream ss;
			ss << "KeyTypedEvent:" << m_KeyCode;
			return ss.str();
#else
			return std::string("非Debug模式未设置，来自eventsystem。")
#endif
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}