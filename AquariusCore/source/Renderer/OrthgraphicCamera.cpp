#include "AQPCH.h"
#include "OrthgraphicCamera.h"
#include <GLM/gtc/matrix_transform.hpp>

namespace Aquarius
{

	OrthgraphicCamera::OrthgraphicCamera(float left, float right, float bottom, float top)
		:m_Projection(glm::ortho(left,right,bottom,top,-0.1f,1.0f)),m_View(1.0f)
	{
		m_VP = m_Projection * m_View;
	}


	void OrthgraphicCamera::CalculateView()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)*
			glm::rotate(glm::mat4 (1.0f),glm::radians(m_Rotation),glm::vec3(0.0f,0.0f,1.0f));
		
		m_View = glm::inverse(transform);
		m_VP = m_Projection * m_View;
	}


}