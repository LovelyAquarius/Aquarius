#pragma once

#include"core/Window.h"
#include "core/Input.h"
#include "EventSystem/ApplicationEvent.h"
#include "core/LayerStack.h"
#include "core/KeyCode.h"



namespace Aquarius
{
	class APPGUI;
	class  AQUARIUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(BaseEvent& event);
		void OnUpdate();
		void OnGUIRender();			//UI渲染时机
		void OnRender();					//layer渲染时机

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);


		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);



		//app核心组件
		static Application* s_Instance;
		std::unique_ptr<Window> m_Window{ Window::Create() };
		APPGUI* m_GUI;
		bool Running = true;
		LayerStack m_LayerStack;
		//______________________________________


		

	};
	Application* CreateApplication();

}


