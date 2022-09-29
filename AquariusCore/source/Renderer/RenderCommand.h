#pragma once
#include "RenderAPI.h"


namespace Aquarius
{

	class AQUARIUS_API RenderCommand
	{
	public:


		 static void SetClearcolor(const glm::vec4& color) ;
		 static void Clear();



		 static void DrawElement(const std::shared_ptr<AQVertexArray>& VAO);
		 static void DrawElement(const std::shared_ptr<AQVertexArray>& VAO, uint32_t elementlocation);
	private:
		static RenderAPI* s_RenderAPI;
	};



}