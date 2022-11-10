#include "AQPCH.h"
#include "OrthgraphicCamera.h"
#include "core/AQCommon.h"
#include <GLM/gtc/matrix_transform.hpp>

namespace Aquarius
{

	OrthgraphicCamera::OrthgraphicCamera(float left, float right, float bottom, float top)
		:m_Projection(),m_View()
	{
		AQ_CORE_EIGEN_IDENTITY_MATRIX4F(1.0f, m_View);
		SetProjection(left, right, bottom, top);
	}


	void OrthgraphicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_Projection = AQ_Ortho(left, right, bottom, top, -0.1f, 1.0f);
		m_VP = m_Projection * m_View; 
	}

	void OrthgraphicCamera::CalculateView()
	{
		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		AQ_CORE_EIGEN_IDENTITY_MATRIX4F(1.0f, transform.matrix());
		transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(m_Rotation), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));
		transform.translate(m_Position);

		m_View = transform.inverse().matrix();
		m_VP = m_Projection * m_View;
	}


}