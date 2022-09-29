#pragma once
#include "core/AquariusCore.h"
#include "AQBuffer.h"

#include <GLM/glm.hpp>
namespace Aquarius
{
	enum class AQUARIUS_API GraphicAPI
	{
		None = 0,
		OpenGL = 1
	};

	class AQUARIUS_API RenderAPI
	{
	public:


	public:
		 static GraphicAPI& GetGraphicAPI() { return s_GraphicAPI; }

		virtual void SetClearcolor(const glm::vec4& color)=0;
		virtual void Clear() = 0;

		virtual void DrawElement(const std::shared_ptr<AQVertexArray>& VAO, uint32_t elementlocation=0)=0;

	private:
		
		static GraphicAPI s_GraphicAPI;
	};

	  
	



}

