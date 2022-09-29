#pragma once
#include "core/AquariusCore.h"

#include <GLM/glm.hpp>

namespace Aquarius
{
	class AQUARIUS_API OrthgraphicCamera
	{
	public:
		OrthgraphicCamera(float left, float right, float bottom, float top);
	
	public:
		const glm::vec3& GetPosition()const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		const float GetRotation()const { return m_Rotation; }
		void SetRotation(const float& rotation) { m_Rotation = rotation; }
		const glm::mat4& GetProjection()const { return m_Projection; }
		const glm::mat4& GetView()const { return m_View; }
		const glm::mat4& GetViewProjection()const { return m_VP; }
	
	public:
		void CalculateView();
	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_VP;
		glm::vec3 m_Position={0.0f,0.0f,0.0f};
		float m_Rotation = 0.0f;

	};




}