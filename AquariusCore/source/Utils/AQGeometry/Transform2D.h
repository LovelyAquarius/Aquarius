#pragma once
#include "core/AquariusCore.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/geometric.hpp>
#include <GLM/gtx/vector_angle.hpp>
#include <GLM/gtx/matrix_transform_2d.hpp>

namespace Aquarius

{
	class AQUARIUS_API GLMTransform2D
	{
	public:
		GLMTransform2D():transform(1.0f){};



	public:
		void Rotate(float& radians);
		void Translate(glm::vec2& translation);
		void Scale(glm::vec2& scale);

		void RotateAboutPoint(float& radians, glm::vec2& point);


		glm::mat3 transform;

	};






}


