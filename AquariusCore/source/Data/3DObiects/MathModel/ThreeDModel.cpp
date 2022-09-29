#include "AQPCH.h"
#include "ThreeDModel.h"

namespace Aquarius
{






	ThreeDModel::ThreeDModel()
		:m_model(glm::mat4(1.0f))
	{

	}

	ThreeDModel::ThreeDModel(glm::mat4& model)
		: m_model(model)
	{
	
	}

	ThreeDModel::~ThreeDModel()
	{
	}

	void ThreeDModel::Rotate(float radians, glm::vec3 about_point)
	{
		m_model = glm::rotate(m_model, radians, about_point);
	}



	void ThreeDModel::Translate(glm::vec3 translation)
	{
		m_model = glm::translate(m_model, translation);
	}


}