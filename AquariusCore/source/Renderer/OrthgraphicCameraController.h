#pragma once
#include "core/AquariusCore.h"
#include "OrthgraphicCamera.h"
#include "core/TimeSystem.h"
#include "EventSystem/ApplicationEvent.h"
#include "EventSystem/KeyEvent.h"
#include "EventSystem/MouseEvent.h"

namespace Aquarius
{
	struct OrthgraphicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return  Right - Left; }
		float GetHeight() { return  Top - Bottom; }
	};

	class AQUARIUS_API OrthgraphicCameraController
	{
	public:
		OrthgraphicCameraController(float aspectratio, bool ratationenable=true);
		void  OnUpdate(DeltaTime& dt);
		void  OnEvent(BaseEvent& event);
		void  OnResize(AQFLOAT width, AQFLOAT height);



		OrthgraphicCamera& GetCamera() { return m_Camera; }
		const OrthgraphicCamera& GetCamera()const { return m_Camera; }
		float GetZoomlevel()const { return m_ZoomLevel; }
		void SetZoomlevel(float level) {m_ZoomLevel = level;}
		const OrthgraphicCameraBounds& GetBounds()const { return m_Bounds; }

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
		OrthgraphicCameraBounds m_Bounds;
		Eigen::Vector3f m_CameraPosition{ 0.0f,0.0f,0.0f };
		float m_CameraMovingSpeed = 1.0f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 10.0f;
	};






}


