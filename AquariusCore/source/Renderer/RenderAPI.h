#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include "AQBuffer.h"
#include "MathSystem/AQEigen.h"

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
		virtual void SetViewport(AQUINT x, AQUINT y, AQUINT width, AQUINT height) = 0;
		virtual void SetClearcolor(const Eigen::Vector4f& color)=0;
		virtual void Clear() = 0;

		virtual void DrawPointElement(const AQRef<AQVertexArray>& VAO, AQFLOAT pointsize, AQUINT elementcount = 0) = 0;
		virtual void DrawPointElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount = 0) = 0;
		virtual void DrawLineElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount = 0, AQFLOAT width = 5.0f) = 0;
		virtual void DrawLineElement(const AQRef<AQVertexArray>& VAO, const std::vector<AQUINT>& elementcounts, AQFLOAT width = 5.0f) = 0;
		virtual void DrawTriangleElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount=0)=0;
		
		virtual void SetLineWidth(const AQFLOAT width)=0;//对于opengl只能外部设置。
	private:
		
		static GraphicAPI s_GraphicAPI;
	};

	  
	



}

