#pragma once
#include "AQCamera.h"
#include "core/TimeSystem.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/MouseEvent.h"

namespace Aquarius
{
	class AQUARIUS_API EditorCamera: public AQCamera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectratio, float nearclip, float farclip);

		void OnUpdate(DeltaTime& dt);
		void OnEvent(BaseEvent& event);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(AQFLOAT distance) {m_Distance = distance;}
		inline float GetFov() const { return m_Fov; }
		inline void SetFov(AQFLOAT fov) {m_Fov = fov; UpdateProjection();}
		inline void SetViewportSize(AQFLOAT width, AQFLOAT height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); UpdateView(); }
		inline Eigen::Vector2ui GetViewportSize() {Eigen::Vector2ui viewport; viewport << (AQUINT)m_ViewportWidth, (AQUINT)m_ViewportHeight; return viewport;}

		const Eigen::Matrix4f& GetView() const {return m_View;}
		Eigen::Matrix4f GetViewProjection() const;
		Eigen::Vector3f GetUpDirection() const;
		Eigen::Vector3f GetRightDirection() const;
		Eigen::Vector3f GetForwardDirection() const;
		const Eigen::Vector3f& GetFocalPoint() const { return m_FocalPoint; }
		const Eigen::Vector3f& GetPosition() const { return m_Position; }
		void SetPosition(const Eigen::Vector3f& position);
		Eigen::Quaternionf GetOrientation() const;
		float GetPitch() const {return m_Pitch;}
		float GetYaw() const { return m_Yaw; }

	private:
		void UpdateProjection();
		void UpdateView();
		AQBOOL OnMouseScroll(MouseScrolledEvent& event);
		void MousePan(const Eigen::Vector2f& dt);
		void MouseRotate(const Eigen::Vector2f& dt);
		void MouseZoom(float dt);
		Eigen::Vector3f CalculatePosition() const;
		std::pair<AQFLOAT, AQFLOAT> PanSpeed() const;
		AQFLOAT RotationSpeed() const;
		AQFLOAT ZoomSpeed() const;
	public:
		AQBOOL m_ViewPortFocusing = true;
	private:
		AQFLOAT m_Fov = 45.0f;
		AQFLOAT m_AspectRatio = 1.778f;
		AQFLOAT m_NearClip = 0.5f;
		AQFLOAT m_FarClip = 1000.0f;
		AQFLOAT m_Distance = 2.0f;
		AQFLOAT m_ViewportWidth=1600.0f;
		AQFLOAT m_ViewportHeight = 900.0f;
		AQFLOAT m_Pitch = 0.0f;
		AQFLOAT m_Yaw = 0.0f;
		Eigen::Vector2f m_InitialMousePosition;
		Eigen::Vector3f m_Position{0.0f,0.0f,-10.0f};
		Eigen::Vector3f m_FocalPoint{ 0.0f,0.0f,0.0f };
		Eigen::Matrix4f m_View;
	};




}


