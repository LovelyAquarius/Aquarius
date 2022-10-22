#include "AQPCH.h"
#include "AQMonitor.h"
#include "core/KeyCode.h"
#include "core/AQObject.h"

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
				AQ_CLIENT_INFO("{0}; ObjectName:{1}; AQObjectType:{2}; RerenceCount:{3}; MemoryLocation:{4};", count, it->second->GetName(), ConvertAQTypeToString(it->second->GetType()), it->second->GetReferenceCount() - 1, it->first);
				count++;
			}
		}
		return false;
	}

	void AQApplicationMonitor::Init()
	{
		AQApplicationMonitor::s_Monitor = new AQApplicationMonitor();
	}

	std::string AQApplicationMonitor::ConvertAQTypeToString(const AQObjectType& type)
	{
		switch (type)
		{
		case(AQObjectType::AQObject):return std::string("AQObject");
		case(AQObjectType::AQShader):return std::string("AQShader");
		case(AQObjectType::AQGLShader):return std::string("AQGLShader");
		case(AQObjectType::AQVertexBuffer):return std::string("AQVertexBuffer");
		case(AQObjectType::AQElementBuffer):return std::string("AQElementBuffer");
		case(AQObjectType::AQVertexArray):return std::string("AQVertexArray");
		case(AQObjectType::AQGLVertexBuffer):return std::string("AQGLVertexBuffer");
		case(AQObjectType::AQGLElementBuffer):return std::string("AQGLElementBuffer");
		case(AQObjectType::AQGLVertexArray):return std::string("AQGLVertexArray");
		case(AQObjectType::AQTexture2D):return std::string("AQTexture2D");
		case(AQObjectType::AQGLTexture2D):return std::string("AQGLTexture2D");
		case(AQObjectType::AQQuadraticBezierCurve2D):return std::string("AQQuadraticBezierCurve2D");
		case(AQObjectType::AQQuadraticBezierShape2D):return std::string("AQQuadraticBezierShape2D");
		}
		return std::string("UnkownType");
	}

	void AQApplicationMonitor::Refresh()
	{
		if (AQApplicationMonitor::Enable)
		{
			for (auto it =AQApplicationMonitor::s_Monitor->AQObjects.begin(); it!= AQApplicationMonitor::s_Monitor->AQObjects.end(); it++)
			{
				if (it->second->GetReferenceCount() == 1)
				{
					it=AQApplicationMonitor::s_Monitor->AQObjects.erase(it);
					if (it == AQApplicationMonitor::s_Monitor->AQObjects.end())
						it--;
				}
			}
		}
	}




}