#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
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

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearcolor(const glm::vec4& color)=0;
		virtual void Clear() = 0;

		virtual void DrawPointElement(const AQRef<AQVertexArray>& VAO, float pointsize, uint32_t elementcount = 0) = 0;
		virtual void DrawPointElement(const AQRef<AQVertexArray>& VAO,uint32_t elementcount = 0) = 0;
		virtual void DrawLineElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount = 0, float width = 5.0f) = 0;
		virtual void DrawLineElement(const AQRef<AQVertexArray>& VAO, const std::vector<uint32_t>& elementcounts, float width = 5.0f) = 0;
		virtual void DrawTriangleElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount=0)=0;
		
		virtual void SetLineWidth(const float width)=0;//对于opengl只能外部设置。
	private:
		
		static GraphicAPI s_GraphicAPI;
	};

	  
	



}

