#include "AQPCH.h"
#include "AQMathEx.h"
#include <math.h>
#include<random>

namespace Aquarius
{
	AQFLOAT AQ_DegreeToRadian(AQFLOAT degrees)
	{
		return degrees * static_cast<AQFLOAT>(0.01745329251994329576923690768489);
	}
	AQDOUBLE AQ_RadianToDegree(AQDOUBLE radians)
	{
		AQ_ASSERT(std::numeric_limits<AQDOUBLE>::is_iec559, "'degrees' only accept floating-point input");

		return radians * static_cast<AQDOUBLE>(57.295779513082320876798154814105);
	}
	AQFLOAT AQ_Lerp(const AQFLOAT f1, const AQFLOAT f2, AQFLOAT t)
	{
		t = std::clamp(t, 0.0f, 1.0f);
		return f1 + f2 * (1.0f - t);
	}
	Eigen::Vector3f AQ_Lerp(const Eigen::Vector3f vec1, const Eigen::Vector3f vec2, AQFLOAT t)
	{
		t = std::clamp(t, 0.0f, 1.0f);
		return Eigen::Vector3f(vec1.x() + vec2.x() * (1.0f - t), vec1.y() + vec2.y() * (1.0f - t), vec1.z() + vec2.z() * (1.0f - t));
	}
	Eigen::Vector4f AQ_Lerp(const Eigen::Vector4f vec1, const Eigen::Vector4f vec2, AQFLOAT t)
	{
		t = std::clamp(t, 0.0f, 1.0f);
		return Eigen::Vector4f(vec1.x() + vec2.x() * (1.0f - t), vec1.y() + vec2.y() * (1.0f - t), vec1.z() + vec2.z() * (1.0f - t), vec1.w() + vec2.w() * (1.0f - t));
	}
	Eigen::Matrix4f AQ_Ortho(AQFLOAT left, AQFLOAT right, AQFLOAT bottom, AQFLOAT top, AQFLOAT znear, AQFLOAT zfar)
	{
		Eigen::Matrix4f result;
		result.setIdentity();
		result(0, 0) = 2.0f / (right - left);
		result(1, 1) = 2.0f / (top - bottom);
		result(2, 2) = -2.0f / (zfar - znear);
		result(3, 0) = -(right + left) / (right - left);
		result(3, 1) = -(top + bottom) / (top - bottom);
		result(3, 2) = -(zfar + znear) / (zfar - znear);
		return result;
	}
	Eigen::Matrix4f AQ_Perspective(float fovy, float aspect, float znear, float zfar)
	{
		//assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovy / 2.0f);
		Eigen::Matrix4f result;
		result.setZero();
		result(0, 0) = 1.0f / (aspect * tanHalfFovy);
		result(1, 1) = 1.0f / (tanHalfFovy);
		result(2, 2) = -(zfar + znear) / (zfar - znear);
		result(2, 3) = -1.0f;
		result(3, 2) = (-2.0f * zfar * znear) / (zfar - znear);
		return result;
	}
	Eigen::Matrix4f& AQ_Scale_Self(Eigen::Matrix4f& mat, Eigen::Vector3f scale)
	{
		mat.col(0) *= scale.x();
		mat.col(1) *= scale.y();
		mat.col(2) *= scale.z();
		return mat;
	}
	const AQFLOAT AQ_Pi_float()
	{
		return (float)std::_Pi;
	}
}