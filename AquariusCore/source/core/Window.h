#pragma once


#include "EventSystem/EventSystem.h"

namespace Aquarius
{
	struct AQUARIUS_API WindowConfigs
	{
		std::string Title;
		AQUINT Width;
		AQUINT Height;

		WindowConfigs(const std::string& title = "Aquarius",
			AQUINT width = 1280,
			AQUINT height = 720)
			:Title(title), Width(width), Height(height) {}
	};


	class AQUARIUS_API Window
	{
	public:
		using EventCallBackFn = std::function<void(BaseEvent&)>;

		virtual ~Window() {}

		virtual void OnUpdade() = 0;

		virtual AQUINT GetWidth() const = 0;
		virtual AQUINT GetHeight() const = 0;

	
		virtual void SetEventCallBack(const EventCallBackFn& callback) = 0;
		virtual void SetSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

	
		virtual void* GetNativeWindow() const = 0;


		static Window* Create(const WindowConfigs& props = WindowConfigs());



	};




}