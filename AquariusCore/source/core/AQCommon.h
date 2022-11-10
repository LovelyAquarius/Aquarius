#pragma once
#include "AquariusCore.h"
#include <string>
#include "MathSystem/AQEigen.h"



#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/constants.hpp>
#include <GLM/gtx/compatibility.hpp>
namespace Aquarius
{
	AQFLOAT AQ_DegreeToRadian(AQFLOAT degrees);
	AQFLOAT AQ_Lerp(const AQFLOAT f1, const AQFLOAT f2, AQFLOAT t);
	Eigen::Vector3f AQ_Lerp(const Eigen::Vector3f vec1, const Eigen::Vector3f vec2, AQFLOAT t);
	Eigen::Vector4f AQ_Lerp(const Eigen::Vector4f vec1, const Eigen::Vector4f vec2, AQFLOAT t);
	Eigen::Matrix4f AQ_Ortho(AQFLOAT left, AQFLOAT right, AQFLOAT bottom, AQFLOAT top, AQFLOAT znear, AQFLOAT zfar);
	Eigen::Matrix4f AQ_Perspective(float fovy, float aspect, float znear, float zfar);
	Eigen::Matrix4f& AQ_Scale_Self(Eigen::Matrix4f& mat, Eigen::Vector3f scale);
	const AQFLOAT AQ_Pi_float();
}
namespace Aquarius
{
	void AQ_Do_Nothing();
	std::string AQ_LoadFile(const std::string& filepath);
	unsigned char* AQ_LoadFile_U8(const std::string& filepath);
	std::string AQ_ExtractFilename(const std::string& filepath);

	std::wstring AQ_UTF8ToUnicode(const std::string& str);
	void AQ_UTF8ToUnicode(const char* u8str,char* unicodestr);



	Eigen::Vector3f CalculateBezierPosition(const Eigen::Vector3f& start, const Eigen::Vector3f& end, const Eigen::Vector3f& controller, AQFLOAT t);
	float CalculateBezierCurvity(const glm::vec3& start, const glm::vec3& end, const glm::vec3& controller, const float t);
	float* CalculateBezierCurve(const Eigen::Vector3f& start, const Eigen::Vector3f& end, const Eigen::Vector3f& controller, AQUINT count);
	 

}
namespace Aquarius
{
	template<int bytecount> struct AQUARIUS_API AQ_N_Byte
	{
		char byte1[bytecount];
	};




	struct AQColorRGBA
	{
		float R, G, B, A;

		AQColorRGBA() :R(1.0f), G(1.0f), B(1.0f), A(1.0f) {}
		AQColorRGBA(float r,float g,float b,float a) :R(r), G(g), B(b), A(a) {}
	};

}
namespace Aquarius
{
	struct AQ2DCoord
	{
		float x, y;

		AQ2DCoord():x(0.0f) ,  y(0.0f) {};
		AQ2DCoord(float x, float y) :x(x), y(y) {};
		AQ2DCoord operator+(const AQ2DCoord& other)
		{
			return AQ2DCoord(this->x+ other.x, this->y + other.y);
		}
		AQ2DCoord& operator+=(const AQ2DCoord& other)
		{
			this->x += other.x;
			this->y += other.y;
			return *this;
		}
		AQ2DCoord operator-(const AQ2DCoord& other)
		{
			return AQ2DCoord(this->x - other.x, this->y - other.y);
		}
		AQ2DCoord operator-()
		{
			return AQ2DCoord(-this->x , -this->y);
		}
		AQ2DCoord operator/(float d)
		{
			return AQ2DCoord(this->x /d, this->y / d);
		}

		AQ2DCoord& operator/=(float d)
		{
			this->x /= d;
			this->y /= d;
			return *this;
		}


	};

	struct AQ3DCoord
	{
		float x, y,z;

		AQ3DCoord() :x(0.0f), y(0.0f) ,z(0.0f) {};
		AQ3DCoord(float x, float y, float z) :x(x), y(y),z(z) {};
	};



}