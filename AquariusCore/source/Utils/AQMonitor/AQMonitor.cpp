#include "AQPCH.h"
#include "AQMonitor.h"
#include "core/KeyCode.h"

namespace Aquarius
{
	

	bool AQApplicationMonitor::Enable = false;
	AQApplicationMonitor* AQApplicationMonitor::s_Monitor = nullptr;

	void AQApplicationMonitor::OnAttach()
	{
		AQApplicationMonitor::Enable = true;
		AQ_CORE_INFO("内存监视器已启用！");
	}

	void AQApplicationMonitor::OnUpdate(DeltaTime& dt)
	{
		m_time += dt;
		if (m_time > 3.0f)
		{
			Refresh();
			m_time = 0.0f;
		}
	}

	void AQApplicationMonitor::OnEvent(BaseEvent& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(AQ_BIND_EVENT_FN(AQApplicationMonitor::LogAllStatus));
	}

	bool AQApplicationMonitor::LogAllStatus(KeyPressedEvent& event)
	{
		if (AQApplicationMonitor::Enable&& event.GetKeyCode()== AQ_KEY_P)
		{
			AQ_CLIENT_INFO("Now exits {0} AQObject",AQApplicationMonitor::s_Monitor->AQObjects.size());
			int count=1;
			for (auto it = AQApplicationMonitor::s_Monitor->AQObjects.begin(); it != AQApplicationMonitor::s_Monitor->AQObjects.end(); it++)
			{
				AQ_CLIENT_INFO("{0}; ObjectName:{1}; AQObjectType:{2}; RerenceCount:{3}; MemoryLocation:{4};", count, it->second->GetName(), (int)it->second->m_type, it->second->GetReferenceCount() - 1, it->first);
				count++;
			}
		}
		return false;
	}

	void AQApplicationMonitor::Init()
	{
		AQApplicationMonitor::s_Monitor = new AQApplicationMonitor();
	}

	void AQApplicationMonitor::Refresh()
	{
		if (AQApplicationMonitor::Enable)
		{
			for (auto it =AQApplicationMonitor::s_Monitor->AQObjects.begin(); it!= AQApplicationMonitor::s_Monitor->AQObjects.end();it++)
			{
				if (it->second->GetReferenceCount() == 1)
				{
					it->second->ReferenceDecrease();
					AQApplicationMonitor::s_Monitor->AQObjects.erase(it);
				}
			}
		}
	}




}