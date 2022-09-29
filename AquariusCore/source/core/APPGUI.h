#pragma once
#include "AquariusCore.h"
#include "UISystem/AQGUI/AQGUI.h"
#include "UISystem/IMGUILayer/IMGUI.h"
#include "core/KeyCode.h"
#include "EventSystem/KeyEvent.h"


#ifdef AQ_IMGUI

#include "IMGUI/imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>






namespace Aquarius
{

	class AQUARIUS_API APPGUI :public IMGUILayer
	{
	public:
		static APPGUI* GetAPPGUI()
		{
			static APPGUI* m_GUI;
			if (m_GUI == nullptr)
				m_GUI = new APPGUI();
			return m_GUI;
		}

		virtual void  OnAttach() override;
		virtual void  OnDetach() override;
		virtual void OnImGuiRender()override;


		void Begin();
		void End();

		virtual void  OnEvent(BaseEvent& event)override;


	private:
		APPGUI() :IMGUILayer("AquariusAppliction") {};

	private:
		float m_Time = 0.0f;



	public:
		//事件声明
		bool OnSpaceButtonPressed(KeyPressedEvent& event);


	};










}



#endif 




#ifdef AQ_AQGUI
namespace Aquarius
{
	//应用程序的GUI

	class AQUARIUS_API APPGUI :public AQGUI
	{
	public:
		static APPGUI* GetAPPGUI()
		{
			static APPGUI* m_GUI;
			if (m_GUI == nullptr)
				m_GUI = new APPGUI();
			return m_GUI;
		}

		void OnRender() override
		{

		}


		void OnUpdate() override
		{
			if (Input::IsKeyPressed(AQ_KEY_TAB))
				AQ_CORE_TRACE("GUI::Tab pressed!");
		}

		void OnEvent(BaseEvent& event) override
		{
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<KeyPressedEvent>(AQ_BIND_EVENT_FN(APPGUI::OnSpaceButtonPressed));


		}

	private:
		APPGUI() :AQGUI("AquariusAppliction") {};


	public:
		//事件声明
		bool OnSpaceButtonPressed(KeyPressedEvent& event);


	};
}
	

#endif // AQ_AQGUI


