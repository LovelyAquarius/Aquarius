#include "AQPCH.h"
#include "Transform2D.h"


namespace Aquarius
{
	void GLMTransform2D::Rotate(float& radians)
	{
		transform=glm::rotate(transform, radians);
	}

	void GLMTransform2D::Translate(glm::vec2& translation)
	{
		transform = glm::translate(transform, translation);
	}

	void GLMTransform2D::Scale(glm::vec2& scale)
	{
		transform = glm::scale(transform, scale);
	}

	void GLMTransform2D::RotateAboutPoint(float& radians, glm::vec2& point)
	{
		transform = glm::translate(transform, -point);
		transform = glm::rotate(transform, radians);
		transform = glm::translate(transform, point);
	}

}