#pragma once
#include"core/AquariusCore.h"
#include "core/Layer.h"
#include "EventSystem/KeyEvent.h"

namespace Aquarius
{
	class AQObject;
	template <typename T> class  AQreference;
	enum class AQObjectType;



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
		static std::string  ConvertAQTypeToString(const AQObjectType& type);
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


