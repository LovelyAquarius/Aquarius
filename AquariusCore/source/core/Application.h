#pragma once
#include "core/Window.h"
#include "core/Input.h"
#include "core/TimeSystem.h"
#include "core/LayerStack.h"
#include "core/KeyCode.h"
#include "EventSystem/ApplicationEvent.h"

namespace Aquarius
{
	class APPGUI;
	class  AQUARIUS_API Application
	{
	public:
		Application(std::string name ="Aquarius");
		virtual ~Application();

		void Run();
		void OnEvent(BaseEvent& event);
		void OnUpdate(DeltaTime& dt);
		void OnGUIRender();			//UI渲染时机
		void OnRender(DeltaTime& dt);					//layer渲染时机

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		APPGUI* GetGUILayer(){ return m_GUI; }
		AQFLOAT& GetLastframeTime() {return m_LastFrameTime;}

		static std::vector<ProfileMessage> s_profile;
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		//app核心组件
		static Application* s_Instance;
		std::unique_ptr<Window> m_Window;
		APPGUI* m_GUI;
		bool Running = true;
		bool Minlized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime;
		
		//______________________________________
		//其他
		DeltaTime zerotime = 0.0f;

	};
	Application* CreateApplication();

}


