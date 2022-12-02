#include "AQPCH.h"
#include "AQCommon.h"
#include "MathSystem/AQMathEx.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"
#include <math.h>
#include<random>
#include<GLM/gtx/norm.hpp>


#pragma warning(disable:4305)
#pragma warning(disable:4996)



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

	std::string AQ_UnicodeToUTF8(const std::wstring& unicodestr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.to_bytes(unicodestr);
	}












}
