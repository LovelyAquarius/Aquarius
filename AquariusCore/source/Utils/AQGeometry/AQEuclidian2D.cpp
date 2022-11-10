#include"AQPCH.h"
#include "AQEuclidian2D.h"
#include "Transform2D.h"

#include <GLM/geometric.hpp>
#include <GLM/gtx/vector_angle.hpp>


namespace Aquarius
{
	AQRef<AQQuadraticBezierCurve2D> AQQuadraticBezierCurve2D::Create()
	{
		return new AQQuadraticBezierCurve2D();
	}
	AQRef<AQQuadraticBezierCurve2D> AQQuadraticBezierCurve2D::Create(const std::string name)
	{
		return  new AQQuadraticBezierCurve2D(name);
	}

	AQ2DCoord AQQuadraticBezierCurve2D::GetCenter()const
	{
		AQ2DCoord sum(0.0f,0.0f);
		for (int i = 0; i < points.size(); i++)
		{
			sum.x += points[0].x;
			sum.y += points[0].y;
		}
		sum.x /= points.size();
		sum.y /= points.size();
		return sum;
	}

	void AQQuadraticBezierCurve2D::SetStartpoint(AQ2DCoord start)
	{
		points[0] = start;
	}

	void AQQuadraticBezierCurve2D::MoveCurve(const AQ2DCoord vector)
	{
		for (int i = 0; i < points.size(); i++)
		{
			points[i] += vector;
		}

		for (int i = 0; i < controls.size(); i++)
		{
			controls[i] += vector;
		}
	}

	void AQQuadraticBezierCurve2D::AddPoint(AQ2DCoord point, AQ2DCoord control)
	{
		if (points.size() == controls.size() + 1)
		{
			points.emplace_back(point);
			controls.emplace_back(control);
		}
		else
		{
			AQ_CORE_WARN("AQQuadraticBezierCurve2D::AddPoint: Failed to AddPoint ,because of the count of linkpoints({0}) does not matches the count of controlpoints({1})，which is from the object '{2}' ", points.size(), controls.size(), this->GetName());
		}
	}

	void AQQuadraticBezierCurve2D::Reset()
	{
		points.clear();
		controls.clear();
		points.resize(1);
		controls.resize(0);
	}


	AQQuadraticBezierCurve2D::AQQuadraticBezierCurve2D()
		:points(std::vector<AQ2DCoord>(1)), controls(std::vector<AQ2DCoord>(0))
	{
		m_Name = "AQQuadraticBezierCurve2D";
		m_type = AQObjectType::AQQuadraticBezierCurve2D;
	}

	AQQuadraticBezierCurve2D::AQQuadraticBezierCurve2D(const std::string& name)
		: points(std::vector<AQ2DCoord>(1)), controls(std::vector<AQ2DCoord>(0))
	{
		m_Name = name;
		m_type = AQObjectType::AQQuadraticBezierCurve2D;
	}

	AQRef<AQQuadraticBezierShape2D> AQQuadraticBezierShape2D::Create()
	{
		return new AQQuadraticBezierShape2D();
	}

	AQRef<AQQuadraticBezierShape2D> AQQuadraticBezierShape2D::Create(const std::string& name)
	{
		return new AQQuadraticBezierShape2D(name);
	}

	void AQQuadraticBezierShape2D::AddPoint(AQ2DCoord point, status status)
	{
		points.emplace_back(point);
		st.emplace_back(status);
	}

	void AQQuadraticBezierShape2D::AddControl(AQ2DCoord control)
	{
		controls.emplace_back(control);
	}

	void AQQuadraticBezierShape2D::Reset()
	{
		points.clear();
		st.clear();
		controls.clear();

	}

	AQQuadraticBezierShape2D::AQQuadraticBezierShape2D()
		: points(std::vector<AQ2DCoord>(0)), st(std::vector<status>(0)),controls(std::vector<AQ2DCoord>(0))
	{
		m_Name = "AQQuadraticBezierShape2D";
		m_type = AQObjectType::AQQuadraticBezierShape2D;
	}

	AQQuadraticBezierShape2D::AQQuadraticBezierShape2D(const std::string& name)
		: points(std::vector<AQ2DCoord>(0)), st(std::vector<status>(0)), controls(std::vector<AQ2DCoord>(0))
	{
		m_Name = name;
		m_type = AQObjectType::AQQuadraticBezierShape2D;
	}

}













namespace Aquarius
{
	Point::Point()
		:CenterPoint(0.0f)
	{

	}
	Point::Point(const glm::vec2& centerposition) : CenterPoint(centerposition,0.0f) {  }
	Point::~Point() {}

	void Point::Translate(glm::vec2& vector)
	{
		GLMTransform2D transform;
		transform.Translate(vector);
		CenterPoint = transform.transform * CenterPoint;
	}



}

namespace Aquarius
{
	StraightLine::StraightLine()
		:StartPoint(0.0f), EndPoint(0.0f), Length(0.0f), Direction(0.0f)
	{
	}
	StraightLine::StraightLine(const glm::vec2& startpoint, const glm::vec2& endpoint)
		: StartPoint(startpoint,0.0f),EndPoint(endpoint,0.0f)
	{
		ComputeCenter();
		ComputeLength();
		ComputeDirection();

	}

	StraightLine::StraightLine(const float& length ,const glm::vec2& centerpoint, glm::vec2& direction)
		: Point(centerpoint),Direction(direction,0.0f), Length(length)
	{
		ComputeEndPointsFromcenter();
	}

	StraightLine::StraightLine(const glm::vec2& startpoint, glm::vec2& direction, const float& length)
		: StartPoint(startpoint,0.0f), Direction(direction,0.0f), Length(length)
	{
		ComputeEndPoint();
		ComputeCenter();
	}

	StraightLine::~StraightLine()
	{
	}

	void StraightLine::ComputeCenter() 
	{
		CenterPoint = (StartPoint + EndPoint) *0.5f;
	}

	void StraightLine::ComputeEndPoint()
	{
		EndPoint = StartPoint + Direction * Length;
	}

	void StraightLine::ComputeEndPointsFromcenter()
	{
		EndPoint = CenterPoint + Direction * Length * 0.5f;
		StartPoint= CenterPoint + Direction * Length * -0.5f;
	}

	void StraightLine::ComputeLength() 
	{
		Length = glm::length(EndPoint - StartPoint);
	}

	void StraightLine::ComputeDirection()
	{
		Direction = glm::normalize(EndPoint - StartPoint);
	}

	void StraightLine::Translate(glm::vec2& vector)
	{
		GLMTransform2D transform;
		transform.Translate(vector);
		CenterPoint = transform.transform * CenterPoint;
		StartPoint = transform.transform * StartPoint;
		EndPoint = transform.transform * EndPoint;
	}


}



namespace Aquarius
{
	Triangle::Triangle()
		:ThirdPoint(0.0f), Area(0.0f)
	{
	}
	Triangle::Triangle(glm::vec2& point1, glm::vec2& point2, glm::vec2& point3)
		:ThirdPoint(point3,0.0f)
	{
		StartPoint = glm::vec3(point1,0.0f);
		EndPoint = glm::vec3(point2, 0.0f);
		ComputeLength();
		ComputeArea();
		ComputeCenter();
	}

	void Triangle::ComputeLength()
	{
		Length = 0.0f;
		Length += glm::length(EndPoint - StartPoint);
		Length += glm::length(StartPoint - ThirdPoint);
		Length += glm::length(ThirdPoint - EndPoint);
	}

	void Triangle::ComputeArea()
	{
		float angle = glm::angle((EndPoint - StartPoint), (ThirdPoint - StartPoint));
		Area = 0.5f * glm::length(EndPoint - StartPoint) * glm::length(ThirdPoint - StartPoint) * glm::sin(angle);
	}

	void Triangle::ComputeCenter()
	{
		CenterPoint = (StartPoint + EndPoint + ThirdPoint) / 3.0f;
	}

	void Triangle::Translate(glm::vec2& vector)
	{
		GLMTransform2D transform;
		transform.Translate(vector);
		CenterPoint = transform.transform * CenterPoint;
		StartPoint = transform.transform * StartPoint;
		EndPoint = transform.transform * EndPoint;
		ThirdPoint = transform.transform * ThirdPoint;
	}

}


namespace Aquarius
{
	Quadrangle::Quadrangle()
		: ForthPoint(0.0f)
	{
	}
	Quadrangle::Quadrangle(glm::vec2& point1, glm::vec2& point2, glm::vec2& point3, glm::vec2& point4)
		:Triangle(point1,point2,point3),ForthPoint(point4,0.0f)

	{
		ComputeLength();
		ComputeArea();
		ComputeCenter();
	}

	void Quadrangle::ComputeLength()
	{
		Length = 0.0f;
		Length+= glm::length(EndPoint - StartPoint);
		Length += glm::length(ThirdPoint - EndPoint);
		Length += glm::length(ForthPoint - ThirdPoint);
		Length += glm::length(StartPoint - ForthPoint);
	}

	void Quadrangle::ComputeArea()
	{
		float angle = glm::angle((EndPoint - ForthPoint), (ThirdPoint - ForthPoint));
		Area += 0.5f * glm::length(EndPoint - ForthPoint) * glm::length(ThirdPoint - ForthPoint) * glm::sin(angle);
	}

	void Quadrangle::ComputeCenter()
	{
		CenterPoint = (StartPoint + EndPoint + ThirdPoint+ForthPoint) / 4.0f;
	}

	void Quadrangle::Translate(glm::vec2& vector)
	{
		GLMTransform2D transform;
		transform.Translate(vector);
		CenterPoint = transform.transform * CenterPoint;
		StartPoint = transform.transform * StartPoint;
		EndPoint = transform.transform * EndPoint;
		ThirdPoint = transform.transform * ThirdPoint;
		ForthPoint = transform.transform * ForthPoint;
	}


}
namespace Aquarius
{
	Rectangle::Rectangle(glm::vec2& point1, glm::vec2& point2)
	{
		StartPoint = glm::vec3(point1,0.0f);
		EndPoint = glm::vec3(point1.x, point2.y, 0.0f);
		ThirdPoint = glm::vec3(point2.x, point1.y, 0.0f);
		ForthPoint = glm::vec3(point2, 0.0f);
		ComputeLength();
		ComputeArea();
		ComputeCenter();

	}

	void Rectangle::ComputeLength()
	{
		Length = 0.0f;
		Length+= glm::length(EndPoint - StartPoint)*2.0f;
		Length += glm::length(ThirdPoint - StartPoint) * 2.0f;
	}

	void Rectangle::ComputeArea()
	{
		Area = 0.0f;
		Area = glm::length(EndPoint - StartPoint) * glm::length(ThirdPoint - StartPoint);
	}

	void Rectangle::ComputeCenter()
	{
		CenterPoint= (StartPoint + ForthPoint) * 0.5f;
	}

	bool Rectangle::IsContained(glm::vec2& point)
	{

		return false;
	}





}