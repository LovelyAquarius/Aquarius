#pragma once
#include "core/AquariusCore.h"


#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

namespace Aquarius
{
	class AQUARIUS_API ThreeDModel
	{
	public:
		ThreeDModel();
		ThreeDModel(glm::mat4 & model);
		~ThreeDModel();

		glm::mat4& Get() { return m_model; }

	public:
		void Rotate(float radians,glm::vec3 about_point);
		
		void Translate(glm::vec3 translation);

		
	private:
		glm::mat4 m_model;
	};






}

