#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include "core/AQCommon.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#define AQ_FLOAT_PRECISION
#ifdef AQ_FLOAT_PRECISION
	 #define PRECISION float
#endif 


namespace Aquarius
{
	//这个类的曲线每一条子曲线都是贝塞尔曲线，且全部相连
	class AQQuadraticBezierCurve2D:public AQObject
	{
	public:
		static AQRef<AQQuadraticBezierCurve2D> Create();
		static AQRef<AQQuadraticBezierCurve2D> Create(const std::string& name);
		static AQObjectType ClassType() { return AQObjectType::AQQuadraticBezierCurve2D; }
	public:
		std::vector<AQ2DCoord>& GetPoints() { return points; }
		const std::vector<AQ2DCoord>& GetPoints() const{ return points; }
		std::vector<AQ2DCoord>& GetControls() { return controls; }
		const std::vector<AQ2DCoord>& GetControls() const { return controls; }
		AQ2DCoord& GetFirstPoint() { return points.front(); }
		const AQ2DCoord& GetFirstPoint()const { return points.front(); }
		AQ2DCoord& GetLastPoint() { return points.back(); }
		const AQ2DCoord& GetLastPoint()const { return points.back(); }

		AQ2DCoord GetCenter()const;

		 void SetStartpoint(AQ2DCoord start);
		 void MoveCurve(const AQ2DCoord vector);

		 void AddPoint(AQ2DCoord point, AQ2DCoord control);
		 void Reset();
		
	protected:
		AQQuadraticBezierCurve2D();
		AQQuadraticBezierCurve2D(const std::string& name);
	protected:
		std::vector<AQ2DCoord> points;
		std::vector<AQ2DCoord> controls;

	};

	//这个类的形状 有直线和曲线，且可以分散开
	class AQQuadraticBezierShape2D :public AQObject
	{
		enum status
		{
			move, line, curve
		};
	public:
		static AQRef<AQQuadraticBezierShape2D> Create();
		static AQRef<AQQuadraticBezierShape2D> Create(const std::string& name);
		static AQObjectType ClassType() { return AQObjectType::AQQuadraticBezierShape2D; }
	public:
		void AddPoint(AQ2DCoord point, status status);
		void AddControl(AQ2DCoord control);
		void Reset();

	protected:
		AQQuadraticBezierShape2D();
		AQQuadraticBezierShape2D(const std::string& name);
	protected:
		std::vector<AQ2DCoord> points;
		std::vector<status> st;
		std::vector<AQ2DCoord> controls;

	};

}















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