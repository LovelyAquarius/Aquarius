#pragma once

#include "AQPCH.h"
#include "core/AquariusCore.h"


namespace Aquarius {

	enum class AQUARIUS_API EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WinndowLostFocus, Windowmoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled

	};

	enum AQUARIUS_API EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),     //000000010
		EventCategoryInput = BIT(1),     //000000100
		EventCategoryKeyboard = BIT(2),     //000001000
		EventCategoryMouse = BIT(3),     //000010000
		EventCategoryMouseButton = BIT(4)	   //000100000

	};




	class  AQUARIUS_API BaseEvent
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual ::std::string ToString() const { return GetName(); }


		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		bool m_Handled = false;
	protected:


	};

	
	class AQUARIUS_API EventDispatcher
	{
		template<typename T>

		using EventFn = ::std::function <bool(T&)>;

	public:
		EventDispatcher(BaseEvent& event) :m_Event(event)
		{

		}


		template<typename T>

		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;

		}
	private:
		BaseEvent& m_Event;
	};


	inline std::ostream& operator<<(std::ostream& os, const BaseEvent& event)
	{
		
		return os << event.ToString();
	}
}

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
							   virtual EventType GetEventType() const override {return GetStaticType();}\
							   virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}