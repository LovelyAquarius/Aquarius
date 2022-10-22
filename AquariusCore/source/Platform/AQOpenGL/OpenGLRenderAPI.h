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
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)override;
		virtual void SetClearcolor(const glm::vec4& color) override;
		virtual void Clear()override;

		virtual void DrawPointElement(const AQRef<AQVertexArray>& VAO, float pointsize, uint32_t elementcount = 0) override;
		virtual void DrawPointElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount = 0) override;
		virtual void DrawTriangleElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount = 0)override;
		virtual void DrawLineElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount = 0, float width = 5.0f)override;
		virtual void DrawLineElement(const AQRef<AQVertexArray>& VAO,const std::vector<uint32_t>& elementcount, float width = 5.0f)override;
		
		virtual void SetLineWidth(const float width) override;
	};


}