#include "AQPCH.h"
#include "OrthgraphicCameraController.h"
#include "core/Input.h"
#include "core/KeyCode.h"

namespace Aquarius
{
	extern AQUINT s_MaxWindowsize = 8192;
	OrthgraphicCameraController::OrthgraphicCameraController(float aspectratio, bool ratationenable)
		:m_AspectRatio(aspectratio), m_RotationEnabled(ratationenable),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Bounds({ -m_AspectRatio * m_ZoomLevel ,m_AspectRatio * m_ZoomLevel ,-m_ZoomLevel, m_ZoomLevel })
	{
	}

	void OrthgraphicCameraController::OnUpdate(DeltaTime& dt)
	{
		//camera控制
		if (Input::IsKeyPressed(AQ_KEY_A))
		{
			
		}
		else if (Input::IsKeyPressed(AQ_KEY_D))
		{
			
		}
		if (Input::IsKeyPressed(AQ_KEY_S))
		{
			
		}	
		else if (Input::IsKeyPressed(AQ_KEY_W))
		{
			
		}
			
		if (m_RotationEnabled)
		{
			if (Input::IsKeyPressed(AQ_KEY_Q))
			{
				m_CameraRotation += m_CameraRotationSpeed * dt;
				m_Camera.SetRotation(m_CameraRotation);
			}
			else if (Input::IsKeyPressed(AQ_KEY_E))
			{
				m_CameraRotation -= m_CameraRotationSpeed * dt;
				m_Camera.SetRotation(m_CameraRotation);
			}
		}
		//___________________________________
	}

	void OrthgraphicCameraController::OnEvent(BaseEvent& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(AQ_BIND_EVENT_FN(OrthgraphicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent> (AQ_BIND_EVENT_FN(OrthgraphicCameraController::OnWindowResized));

	}

	void OrthgraphicCameraController::OnResize(AQFLOAT width, AQFLOAT height)
	{
		m_AspectRatio = width/ height;
		m_Bounds = { -m_AspectRatio * m_ZoomLevel ,m_AspectRatio * m_ZoomLevel ,-m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthgraphicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetyOffset()*0.1f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.2f);
		m_CameraMovingSpeed = m_ZoomLevel;
		m_Bounds = { -m_AspectRatio * m_ZoomLevel ,m_AspectRatio * m_ZoomLevel ,-m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		return false;
	}

	bool OrthgraphicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		AQUINT width = event.GetWidth();
		AQUINT height = event.GetHeight();
		if (width < s_MaxWindowsize && height < s_MaxWindowsize)
		{
			m_AspectRatio = (float)width / (float)height;
			m_Bounds = { -m_AspectRatio * m_ZoomLevel ,m_AspectRatio * m_ZoomLevel ,-m_ZoomLevel, m_ZoomLevel };
			m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		}
		
		return false;
	}

}











