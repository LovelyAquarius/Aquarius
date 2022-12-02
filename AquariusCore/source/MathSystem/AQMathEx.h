#pragma once
#include "core/AquariusCore.h"
#include "AQEigen.h"
namespace Eigen
{
	typedef Eigen::Matrix<unsigned int, 2, 1> Vector2ui;
}

namespace Aquarius
{
	AQFLOAT AQ_DegreeToRadian(AQFLOAT degrees);
	AQDOUBLE AQ_RadianToDegree(AQDOUBLE radians);
	AQFLOAT AQ_Lerp(const AQFLOAT f1, const AQFLOAT f2, AQFLOAT t);
	Eigen::Vector3f AQ_Lerp(const Eigen::Vector3f vec1, const Eigen::Vector3f vec2, AQFLOAT t);
	Eigen::Vector4f AQ_Lerp(const Eigen::Vector4f vec1, const Eigen::Vector4f vec2, AQFLOAT t);
	Eigen::Matrix4f AQ_Ortho(AQFLOAT left, AQFLOAT right, AQFLOAT bottom, AQFLOAT top, AQFLOAT znear, AQFLOAT zfar);
	Eigen::Matrix4f AQ_Perspective(float fovy, float aspect, float znear, float zfar);
	Eigen::Matrix4f& AQ_Scale_Self(Eigen::Matrix4f& mat, Eigen::Vector3f scale);
	const AQFLOAT AQ_Pi_float();

	
}