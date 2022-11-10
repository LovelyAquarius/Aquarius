#pragma once
#pragma once
#include "core/AquariusCore.h"
#include "Renderer/RenderAPI.h"

namespace Aquarius
{
	class AQUARIUS_API OpenGLRenderAPI :public RenderAPI
	{
	public:
		virtual void Init()override;
		virtual void SetViewport(AQUINT x, AQUINT y, AQUINT width, AQUINT height)override;
		virtual void SetClearcolor(const Eigen::Vector4f& color) override;
		virtual void Clear()override;

		virtual void DrawPointElement(const AQRef<AQVertexArray>& VAO, AQFLOAT pointsize, AQUINT elementcount = 0) override;
		virtual void DrawPointElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount = 0) override;
		virtual void DrawTriangleElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount = 0)override;
		virtual void DrawLineElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount = 0, AQFLOAT width = 5.0f)override;
		virtual void DrawLineElement(const AQRef<AQVertexArray>& VAO,const std::vector<AQUINT>& elementcount, AQFLOAT width = 5.0f)override;
		
		virtual void SetLineWidth(const AQFLOAT width) override;
	};


}