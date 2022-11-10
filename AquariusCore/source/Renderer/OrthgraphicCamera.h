#pragma once
#include "core/AquariusCore.h"
#include "MathSystem/AQEigen.h"

namespace Aquarius
{

	class AQUARIUS_API OrthgraphicCamera
	{
	public:
		OrthgraphicCamera(float left, float right, float bottom, float top);
	
	public:
		const Eigen::Vector3f& GetPosition()const { return m_Position; }
		void SetPosition(const Eigen::Vector3f& position) { m_Position = position; CalculateView(); }
		const float GetRotation()const { return m_Rotation; }
		void SetRotation(const float& rotation) { m_Rotation = rotation;  CalculateView();}
		const Eigen::Vector2f& GetDirection()const { return m_Direction; }
		void SetDirection(const Eigen::Vector2f& direction) { m_Direction = direction;  CalculateView();}
		const Eigen::Matrix4f& GetProjection()const { return m_Projection; }
		void SetProjection(float left, float right, float bottom, float top);
		const Eigen::Matrix4f& GetView()const { return m_View; }
		const Eigen::Matrix4f& GetViewProjection()const { return m_VP; }
	
	public:
		void CalculateView();
	private:
		Eigen::Matrix4f m_Projection;
		Eigen::Matrix4f m_View;
		Eigen::Matrix4f m_VP;
		Eigen::Vector3f m_Position={0.0f,0.0f,0.0f};
		Eigen::Vector2f m_Direction = { 0.0f,0.0f };
		float m_Rotation = 0.0f;

	};



}