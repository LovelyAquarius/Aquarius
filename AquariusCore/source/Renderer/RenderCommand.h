#pragma once
#include "RenderAPI.h"


namespace Aquarius
{

	class AQUARIUS_API RenderCommand
	{
	public:

		inline static void Init();
		inline static void SetViewport(uint32_t x, uint32_t y,uint32_t width, uint32_t height);
		inline static void SetClearcolor(const glm::vec4& color);
		inline static void Clear();

		inline static void DrawPointElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount=0);
		inline static void DrawPointElement(const AQRef<AQVertexArray>& VAO, float pointsize, uint32_t elementcount = 0);
		inline static void DrawTriangleElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount=0);
		inline static void DrawLineElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount = 0, float width = 5.0f);
		inline static void DrawLineElement(const AQRef<AQVertexArray>& VAO, const std::vector<uint32_t>& elementcounts, float width = 5.0f);

		inline static void SetLineWidth(const float width);
	private:
		static RenderAPI* s_RenderAPI;
	};
}

namespace Aquarius
{
	inline void RenderCommand::Init()
	{
		s_RenderAPI->Init();
	}

	inline  void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RenderAPI->SetViewport(x, y, width, height);
	}

	inline void RenderCommand::SetClearcolor(const glm::vec4& color)
	{
		s_RenderAPI->SetClearcolor(color);
	}

	inline void RenderCommand::Clear()
	{
		s_RenderAPI->Clear();
	}


	inline void RenderCommand::DrawPointElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount)
	{
		VAO->Bind();
		s_RenderAPI->DrawPointElement(VAO, elementcount);
	}

	inline void RenderCommand::DrawPointElement(const AQRef<AQVertexArray>& VAO, float pointsize, uint32_t elementcount)
	{
		VAO->Bind();
		s_RenderAPI->DrawPointElement(VAO, pointsize, elementcount);
	}

	inline void RenderCommand::DrawTriangleElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount)
	{
		VAO->Bind();
		s_RenderAPI->DrawTriangleElement(VAO, elementcount);
	}

	inline void RenderCommand::DrawLineElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount, float width)
	{
		VAO->Bind();
		s_RenderAPI->DrawLineElement(VAO, elementcount, width);
	}

	inline void RenderCommand::DrawLineElement(const AQRef<AQVertexArray>& VAO, const std::vector<uint32_t>& elementcounts, float width)
	{
		VAO->Bind();
		s_RenderAPI->DrawLineElement(VAO, elementcounts, width);
	}

	inline void RenderCommand::SetLineWidth(const float width)
	{
		s_RenderAPI->SetLineWidth(width);
	}

}