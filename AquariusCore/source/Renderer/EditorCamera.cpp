#include "AQPCH.h"
#include "core/Input.h"
#include "core/KeyCode.h"
#include "EditorCamera.h"


namespace Aquarius
{

	EditorCamera::EditorCamera(float fov, float aspectratio, float nearclip, float farclip)
		:m_Fov(fov), m_AspectRatio(aspectratio), m_NearClip(nearclip), m_FarClip(farclip)
	{
		m_InitialMousePosition << Input::GetMouseX(), Input::GetMouseY();
		UpdateView();
	}

	void EditorCamera::OnUpdate(DeltaTime& dt)
	{
		
		if (Input::IsKeyPressed(AQ_KEY_LEFT_ALT)&& m_ViewPortFocusing)
		{
			Eigen::Vector2f mouseposition;
			mouseposition << Input::GetMouseX(), Input::GetMouseY();
			Eigen::Vector2f delta = (mouseposition - m_InitialMousePosition) * 0.03f;
			m_InitialMousePosition = mouseposition;
			if (Input::IsKeyPressed(AQ_KEY_W)&& Input::IsMouseButtonPressed(AQ_MOUSE_BUTTON_LEFT))
				MouseZoom(delta.y());
			else if (Input::IsMouseButtonPressed(AQ_MOUSE_BUTTON_LEFT))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(AQ_MOUSE_BUTTON_RIGHT))
				MouseRotate(delta);
		}
		UpdateView();
	}

	void EditorCamera::OnEvent(BaseEvent& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(AQ_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	Eigen::Matrix4f EditorCamera::GetViewProjection() const
	{
		Eigen::Matrix4f result = m_Projection * m_View;
		//result.col(3).w() = 0.0f;
		return result;
	}

	Eigen::Vector3f EditorCamera::GetUpDirection() const
	{
		return GetOrientation() * Eigen::Vector3f::UnitY();
	}

	Eigen::Vector3f EditorCamera::GetRightDirection() const
	{
		return GetOrientation() * Eigen::Vector3f::UnitX();
	}

	Eigen::Vector3f EditorCamera::GetForwardDirection() const
	{
		return GetOrientation() * -Eigen::Vector3f::UnitZ();
	}

	Eigen::Quaternionf EditorCamera::GetOrientation() const
	{
		auto quat =
			Eigen::AngleAxis<AQFLOAT>(-m_Pitch, Eigen::Vector3f::UnitX()) *
			Eigen::AngleAxis<AQFLOAT>(-m_Yaw, Eigen::Vector3f::UnitY()) *
			Eigen::AngleAxis<AQFLOAT>(0.0f, Eigen::Vector3f::UnitZ());
		return quat;
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = AQ_Perspective(AQ_DegreeToRadian(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_Position = CalculatePosition();
		Eigen::Quaternionf orientation = GetOrientation();
		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		transform.setIdentity();
		transform.translate(m_Position) *= orientation;
		m_View = transform.inverse().matrix();
	}
	void EditorCamera::SetPosition(const Eigen::Vector3f& position)
	{
		m_FocalPoint << position.x(),position.y(),0.0f;
		m_Distance = position.z();
		UpdateView();
	}
	AQBOOL EditorCamera::OnMouseScroll(MouseScrolledEvent& event)
	{
		float delta = event.GetyOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const Eigen::Vector2f& dt)
	{
		auto [xspeed, yspeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * dt.x() * xspeed * m_Distance;
		m_FocalPoint += GetUpDirection() * dt.y() * yspeed * m_Distance;

	}

	void EditorCamera::MouseRotate(const Eigen::Vector2f& dt)
	{
		float yawsign = GetUpDirection().y() < 0 ? -1.0f : 1.0f;
		m_Yaw += yawsign * dt.x() * RotationSpeed();
		m_Pitch += dt.y() * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float dt)
	{
		m_Distance -= dt * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_Distance = 1.0f;
		}
	}

	Eigen::Vector3f EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<AQFLOAT, AQFLOAT> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);
		float xfactor = 0.0366f * (x * x) - 0.1778f * x + 0.2021f;
		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);
		float yfactor = 0.0366f * (y * y) - 0.1778f * y + 0.2021f;
		return {xfactor, yfactor };
	}

	AQFLOAT EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	AQFLOAT EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.3f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}

}
