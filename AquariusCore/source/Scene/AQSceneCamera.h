#pragma once
#include "Renderer/AQCamera.h"
#include "core/AQCommon.h"

namespace Aquarius
{
	class AQSceneCamera:public AQCamera
	{
	public:
		enum class ProjectionType { Orthographic=0 ,Perspective = 1};
	public:
		AQSceneCamera();
		AQSceneCamera(const Eigen::Matrix4f& projection);
		virtual ~AQSceneCamera()=default;

		void SetOrthoGraphic(AQFLOAT size, AQFLOAT nearclip, AQFLOAT farclip);
		void SetPerspective(AQFLOAT verticalfov, AQFLOAT nearclip, AQFLOAT farclip);

		void SetViewportSize(AQFLOAT width, AQFLOAT height);
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }
		ProjectionType GetProjectionType() const { return m_ProjectionType; }

		void SetOrthographicSize(AQFLOAT size){m_OrthoGraphicSize = size;RecaculateProjection();}
		AQFLOAT GetOrthographicSize()const { return m_OrthoGraphicSize; }
		void SetOrthographicNear(AQFLOAT nearclip) { m_OrthoGraphicNear = nearclip; RecaculateProjection(); }
		AQFLOAT GetOrthographicNear()const { return m_OrthoGraphicNear;}
		void SetOrthographicFar(AQFLOAT farclip) {m_OrthoGraphicFar = farclip; RecaculateProjection();}
		AQFLOAT GetOrthographicFar()const { return m_OrthoGraphicFar; }

		void SetPerspectiveFov(AQFLOAT verticalfov) { m_PerspectiveFov = verticalfov; RecaculateProjection();}
		AQFLOAT GetPerspectiveFov()const { return m_PerspectiveFov; }
		void SetPerspectiveNear(AQFLOAT nearclip) { m_PerspectiveNear = nearclip; RecaculateProjection(); }
		AQFLOAT GetPerspectiveNear()const { return m_PerspectiveNear; }
		void SetPerspectiveFar(AQFLOAT farclip) { m_PerspectiveFar = farclip; RecaculateProjection(); }
		AQFLOAT GetPerspectiveFar()const { return m_PerspectiveFar; }
		void SetAspectRatio(AQFLOAT aspectratio) {m_AspectRatio= aspectratio; RecaculateProjection();}
		AQFLOAT GetAspectRatio() const { return m_AspectRatio; }

	private:
		void RecaculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
		AQFLOAT m_OrthoGraphicSize = 10.0f;
		AQFLOAT m_OrthoGraphicNear = -1.0f;
		AQFLOAT m_OrthoGraphicFar = 1.0f;

		AQFLOAT m_PerspectiveFov =AQ_DegreeToRadian(45.0f);
		AQFLOAT m_PerspectiveNear = 0.01f;
		AQFLOAT m_PerspectiveFar = 1000.0f;

		AQFLOAT m_AspectRatio = 0.0f;
	};
}


