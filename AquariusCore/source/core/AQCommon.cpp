#include "AQPCH.h"
#include "AQCommon.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"
#include <math.h>
#include<random>
#include<GLM/gtx/norm.hpp>


#pragma warning(disable:4305)
#pragma warning(disable:4996)

namespace Aquarius
{
	AQFLOAT AQ_DegreeToRadian(AQFLOAT degrees)
	{
		return degrees * static_cast<AQFLOAT>(0.01745329251994329576923690768489);
	}
	AQFLOAT AQ_Lerp(const AQFLOAT f1, const AQFLOAT f2, AQFLOAT t)
	{
		t = std::clamp(t, 0.0f, 1.0f);
		return f1+ f2*(1.0f-t);
	}
	Eigen::Vector3f AQ_Lerp(const Eigen::Vector3f vec1, const Eigen::Vector3f vec2, AQFLOAT t)
	{
		t = std::clamp(t, 0.0f, 1.0f);
		return Eigen::Vector3f(vec1.x()+ vec2.x()*(1.0f-t), vec1.y() + vec2.y() * (1.0f - t), vec1.z() + vec2.z() * (1.0f - t));
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
		result(0,0) = 2.0f / (right - left);
		result(1,1) =2.0f / (top - bottom);
		result(2,2) = -2.0f / (zfar - znear);
		result(3,0) = -(right + left) / (right - left);
		result(3,1) = -(top + bottom) / (top - bottom);
		result(3,2) = -(zfar + znear) / (zfar - znear);
		return result;
	}
	Eigen::Matrix4f AQ_Perspective(float fovy, float aspect, float znear, float zfar)
	{
		assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovy / 2.0f);
		Eigen::Matrix4f result;
		result.setZero();
		result(0, 0) = 1.0f / (aspect * tanHalfFovy);
		result(1, 1) = 1.0f / (tanHalfFovy);
		result(2, 2) = -(zfar + znear) / (zfar - znear);
		result(2, 3) = -1.0f;
		result(3, 2) = (-2.0f * zfar * znear) / (zfar - znear);
		result(3, 3) = 1.0f;
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
		return std::_Pi;
	}
}

namespace Aquarius
{
	void AQ_Do_Nothing(){}

	std::string AQ_LoadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			AQ_CORE_ERROR("Failed to open the file:{0}", filepath);
		}
		return result;
	}

	unsigned char* AQ_LoadFile_U8(const std::string& filepath)
	{
		long int size = 0;
		unsigned char* result = nullptr;

		FILE* file = fopen(filepath.c_str(), "rb");
		if (file)
		{
			fseek(file, 0, SEEK_END);
			size = ftell(file);
			fseek(file, 0, SEEK_SET);
			result = (unsigned char*)new  char[size];
			fread(result, size, 1, file);
			fclose(file);
		}
		else
		{
			AQ_CORE_ERROR("Failed to open the file:{0}", filepath);
		}
		return result;
	}



    std::string AQ_ExtractFilename(const std::string& filepath)
    {
        //assets/filename.file,我们要抓取filename这个字符串
        auto lastslash = filepath.find_last_of("/\\");
        lastslash = lastslash == std::string::npos ? 0 : lastslash + 1;
        auto lastdot = filepath.rfind('.');
        auto count = lastdot == std::string::npos ? filepath.size() - lastslash : lastdot - lastslash;

        return filepath.substr(lastslash, count);
    }


	Eigen::Vector3f CalculateBezierPosition(const Eigen::Vector3f& start, const Eigen::Vector3f& end, const Eigen::Vector3f& controller, AQFLOAT t)
	{
		Eigen::Vector3f point1 = AQ_Lerp(start, controller, t);
		Eigen::Vector3f point2 = AQ_Lerp(controller, end, t);
		return  AQ_Lerp(point1, point2, t);
	}

	float CalculateBezierCurvity(const glm::vec3& start, const glm::vec3& end, const glm::vec3& controller, const float t)
	{
		glm::vec3 firstderivative =    glm::vec3{ (2 * t - 2) * start.x + (2 - 4 * t) * controller.x + 2 * t * end.x,(2 * t - 2) * start.y + (2 - 4 * t) * controller.y + 2 * t * end.y ,(2 * t - 2) * start.z + (2 - 4 * t) * controller.z + 2 * t * end.z };
		glm::vec3 secondderivative = glm::vec3{ 2 * start.x -4 * controller.x + 2 * end.x,2 * start.y - 4 * controller.y + 2 * end.y, 2 * start.z - 4 * controller.z + 2 * end.z };

		float norm = glm::l2Norm(secondderivative);
		
		float det = firstderivative.x * secondderivative.y - firstderivative.y - secondderivative.t;
		float result = det / norm / norm / norm;

		return result;
	}

	float* CalculateBezierCurve(const Eigen::Vector3f& start, const Eigen::Vector3f& end, const Eigen::Vector3f& controller, AQUINT count)
	{
		float step = 1.0f / (float)count;
		float t = 0.0f;
		float* data = new float[count * 3];
		Eigen::Vector3f position;
		for (AQUINT index = 0; index < count * 3; index += 3)
		{
			position = CalculateBezierPosition(start, end, controller, t);
			data[index + 0] = position.x();
			data[index + 1] = position.y();
			data[index + 2] = position.z();
			t += step;
		}
		return data;
	}

    

	std::wstring AQ_UTF8ToUnicode(const std::string& str)
	{
		std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.from_bytes(str);
	}
	void AQ_UTF8ToUnicode(const char* u8str, char* unicodestr)
	{
		std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
		std::wstring unicodewstring = strCnv.from_bytes(u8str);
		wcscpy_s((wchar_t*)unicodestr, (unicodewstring.size() + 1)*sizeof(wchar_t), unicodewstring.c_str());
	}












}
