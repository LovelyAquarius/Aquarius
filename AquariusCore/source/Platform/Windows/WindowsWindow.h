#pragma once


#include"Platform/AQOpenGL/AQGLContext.h"
#include	 "core/Window.h"
#include "Platform/Windows/WindowsInput.h"
namespace Aquarius
{
	class AQUARIUS_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowConfigs& props);
		virtual ~WindowsWindow();


		virtual void OnUpdade() override;
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }



		virtual void SetEventCallBack(const EventCallBackFn& callback) override
		{
			m_Data.EventCallBack = callback;
		}


		virtual void SetSync(bool enabled) override;
		virtual bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }


	private:

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallBackFn EventCallBack;
		};


		virtual void Init(const WindowConfigs& props);
		virtual void ShutDown();


		GLFWwindow* m_Window;


		WindowData m_Data;


		GraphicContext* m_Context;

	};








}

