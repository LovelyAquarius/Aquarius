#pragma once
#include"core/AquariusCore.h"
#include"core/AQObject.h"
#include "core/Layer.h"
#include "EventSystem/KeyEvent.h"

namespace Aquarius
{
	class AQObject;
	class AQShder;
	class AQGLShder;
	class AQVertexBuffer;
	class AQGLVertexBuffer;
	class AQElementBuffer;
	class AQGLElementBuffer;
	class AQVertexArray;
	class AQGLVertexArray;

	template <typename T> class  AQreference;

	//AQ程序用于监控各种对象内存情况的类
	class AQUARIUS_API AQApplicationMonitor:public Layer
	{
	public:
		virtual void	OnAttach()override;
		virtual void OnUpdate(DeltaTime& dt)override;
		virtual void OnEvent(BaseEvent& event)override;

	private:
		//控制事件
		bool  LogAllStatus(KeyPressedEvent& event);

	public:
		static void Init();
		static AQApplicationMonitor* s_Monitor;
		static bool Enable;
	private:
	    AQApplicationMonitor():Layer(std::string("APPMonitor")), m_time(0.0f) {}
		void Refresh();
		float m_time;
	public:
		std::unordered_map<void*, AQreference<AQObject>> AQObjects;

	};


	





}


