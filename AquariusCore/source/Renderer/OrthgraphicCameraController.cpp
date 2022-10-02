#include "AQPCH.h"
#include "OrthgraphicCameraController.h"
#include "core/Input.h"
#include "core/KeyCode.h"

namespace Aquarius
{

	OrthgraphicCameraController::OrthgraphicCameraController(float aspectratio, bool ratationenable)
		:m_AspectRatio(aspectratio), m_RotationEnabled(ratationenable),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthgraphicCameraController::OnUpdate(DeltaTime& dt)
	{
		//camera控制
		if (Input::IsKeyPressed(AQ_KEY_A))
		{
			m_CameraPosition.x -= m_CameraMovingSpeed * dt;
			m_Camera.SetPosition(m_CameraPosition);
		}
		else if (Input::IsKeyPressed(AQ_KEY_D))
		{
			m_CameraPosition.x += m_CameraMovingSpeed * dt;
			m_Camera.SetPosition(m_CameraPosition);
		}
		if (Input::IsKeyPressed(AQ_KEY_S))
		{
			m_CameraPosition.y -= m_CameraMovingSpeed * dt;
			m_Camera.SetPosition(m_CameraPosition);
		}	
		else if (Input::IsKeyPressed(AQ_KEY_W))
		{
			m_CameraPosition.y += m_CameraMovingSpeed * dt;
			m_Camera.SetPosition(m_CameraPosition);
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

	bool OrthgraphicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetyOffset()*0.1f;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthgraphicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_AspectRatio = event.GetWidth() / event.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}











