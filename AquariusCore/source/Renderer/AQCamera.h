#pragma once
#include "core/AquariusCore.h"
#include "MathSystem/AQEigen.h"

namespace Aquarius
{
	class AQUARIUS_API AQCamera
	{
	public:
		AQCamera() = default;
		AQCamera(const Eigen::Matrix4f& projection) :m_Projection(projection) {}
		virtual ~AQCamera() = default;

		const Eigen::Matrix4f& GetProjection() const {return m_Projection;}
	protected:
		Eigen::Matrix4f m_Projection= Eigen::Matrix4f().setIdentity();
	};



}

