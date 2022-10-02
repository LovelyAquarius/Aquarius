#pragma once
#include "core/AquariusCore.h"
#include "OrthgraphicCamera.h"
#include "core/TimeSystem.h"
#include "EventSystem/ApplicationEvent.h"
#include "EventSystem/KeyEvent.h"
#include "EventSystem/MouseEvent.h"

namespace Aquarius
{

	class AQUARIUS_API OrthgraphicCameraController
	{
	public:
		OrthgraphicCameraController(float aspectratio, bool ratationenable=true);
		OrthgraphicCamera& GetCamera() { return m_Camera; }
		const OrthgraphicCamera& GetCamera()const { return m_Camera; }

		void  OnUpdate(DeltaTime& dt);
		void  OnEvent(BaseEvent& event);
	private:
		//事件
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_RotationEnabled;
	private:
		OrthgraphicCamera m_Camera;
		glm::vec3 m_CameraPosition{ 0.0f,0.0f,0.0f };
		float m_CameraMovingSpeed = 1.0f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 1.0f;
	};






}


