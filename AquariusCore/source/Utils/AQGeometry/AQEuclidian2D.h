#pragma once
#include "core/AquariusCore.h"



#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#define AQ_FLOAT_PRECISION
#ifdef AQ_FLOAT_PRECISION
	 #define PRECISION float
#endif 

#ifdef AQ_GLM

namespace Aquarius
{
	class AQUARIUS_API Point
	{
	public:
		Point();
		Point(const glm::vec2& centerposition);
		~Point();

	public:
		 virtual void Translate(glm::vec2& vector);

	public:
		//属性
		glm::vec3 CenterPoint;
	private:

	};

	
	class AQUARIUS_API StraightLine: public Point
	{
	public:
		StraightLine();
		StraightLine(const glm::vec2& startpoint, const glm::vec2& endpoint);
		StraightLine(const float& length ,const glm::vec2& centerpoint, glm::vec2& direction);
		StraightLine(const glm::vec2& startpoint, glm::vec2& direction,const float& length);
		~StraightLine();
	public:
		//访问
		virtual float& GetLength() { return Length; }
		virtual glm::vec3& GetDirection()  { return Direction; }
		//方法
		virtual void ComputeCenter();
		virtual void ComputeEndPoint();
		virtual void ComputeEndPointsFromcenter();
		virtual void ComputeLength();
		virtual void ComputeDirection();

		virtual void Translate(glm::vec2& vector);
	public:
		//属性
		glm::vec3 StartPoint;
		glm::vec3 EndPoint;
		float Length;

	private:
		//属性
		glm::vec3 Direction;
		
	};


	class AQUARIUS_API Triangle : public StraightLine
	{

	public:
		Triangle();
		Triangle(glm::vec2& point1, glm::vec2& point2, glm::vec2& point3);


	public:
		//方法
		virtual void ComputeLength();
		virtual void ComputeArea();
		//计算重心
		virtual void ComputeCenter();

		virtual void Translate(glm::vec2& vector);
	public:
		//属性
		glm::vec3 ThirdPoint;
		float Area;
	private:
	};

	class AQUARIUS_API Quadrangle : public Triangle
	{
	public:
		Quadrangle();
		Quadrangle(glm::vec2& point1, glm::vec2& point2, glm::vec2& point3, glm::vec2& point4);


	public:
		//方法
		virtual void ComputeLength();
		virtual void ComputeArea();
		//计算重心
		virtual void ComputeCenter();


		virtual void Translate(glm::vec2& vector);
	public:
		glm::vec3 ForthPoint;



	};

	class AQUARIUS_API Rectangle : public Quadrangle
	{
	public:
		Rectangle(glm::vec2& point1, glm::vec2& point2);

	public:
		//方法
		virtual void ComputeLength();
		virtual void ComputeArea();
		//计算重心
		virtual void ComputeCenter();

		//判断某个点是否在矩形内
		virtual bool  IsContained(glm::vec2& point);

	};

}
#endif 