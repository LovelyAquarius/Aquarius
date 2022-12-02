#include "AQPCH.h"
#include "AQSceneCamera.h"


namespace Aquarius
{





	AQSceneCamera::AQSceneCamera()
	{
		m_Projection = AQ_Ortho(-16.0f, 16.0f, -1.0f, 1.0f, -9.0f, 9.0f);
	}

	AQSceneCamera::AQSceneCamera(const Eigen::Matrix4f& projection)
	{
		m_Projection = projection;
	}



	void AQSceneCamera::SetOrthoGraphic(AQFLOAT size, AQFLOAT nearclip, AQFLOAT farclip)
	{
		m_ProjectionType = ProjectionType::Orthographic;

		m_OrthoGraphicSize = size;
		m_OrthoGraphicNear = nearclip;
		m_OrthoGraphicFar = farclip;
		RecaculateProjection();
	}

	void AQSceneCamera::SetPerspective(AQFLOAT verticalfov, AQFLOAT nearclip, AQFLOAT farclip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFov = verticalfov;
		m_PerspectiveNear = nearclip;
		m_PerspectiveFar = farclip;
		RecaculateProjection();
	}

	void AQSceneCamera::SetViewportSize(AQFLOAT width, AQFLOAT height)
	{
		m_AspectRatio = (AQFLOAT)width / (AQFLOAT)height;
		RecaculateProjection();
	}


	void AQSceneCamera::RecaculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float ortholeft = -0.5f * m_AspectRatio * m_OrthoGraphicSize;
			float orthoright = 0.5f * m_AspectRatio * m_OrthoGraphicSize;
			float orthobottom = -0.5f * m_OrthoGraphicSize;
			float orthotop = 0.5f * m_OrthoGraphicSize;

			m_Projection = AQ_Ortho(ortholeft, orthoright, orthobottom, orthotop, m_OrthoGraphicNear, m_OrthoGraphicFar);
		}
		else 
		{
			m_Projection = AQ_Perspective(m_PerspectiveFov, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}


	}


}