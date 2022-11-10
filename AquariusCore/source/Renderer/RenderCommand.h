#pragma once
#include "RenderAPI.h"


namespace Aquarius
{

	class AQUARIUS_API RenderCommand
	{
	public:

		inline static void Init();
		inline static void SetViewport(AQUINT x, AQUINT y,AQUINT width, AQUINT height);
		inline static void SetClearcolor(const Eigen::Vector4f& color);
		inline static void Clear();

		inline static void DrawPointElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount=0);
		inline static void DrawPointElement(const AQRef<AQVertexArray>& VAO, AQFLOAT pointsize, AQUINT elementcount = 0);
		inline static void DrawTriangleElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount=0);
		inline static void DrawLineElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount = 0, AQFLOAT width = 5.0f);
		inline static void DrawLineElement(const AQRef<AQVertexArray>& VAO, const std::vector<AQUINT>& elementcounts, AQFLOAT width = 5.0f);

		inline static void SetLineWidth(const AQFLOAT width);
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

	inline  void RenderCommand::SetViewport(AQUINT x, AQUINT y, AQUINT width, AQUINT height)
	{
		s_RenderAPI->SetViewport(x, y, width, height);
	}

	inline void RenderCommand::SetClearcolor(const Eigen::Vector4f& color)
	{
		s_RenderAPI->SetClearcolor(color);
	}

	inline void RenderCommand::Clear()
	{
		s_RenderAPI->Clear();
	}


	inline void RenderCommand::DrawPointElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount)
	{
		VAO->Bind();
		s_RenderAPI->DrawPointElement(VAO, elementcount);
	}

	inline void RenderCommand::DrawPointElement(const AQRef<AQVertexArray>& VAO, AQFLOAT pointsize, AQUINT elementcount)
	{
		VAO->Bind();
		s_RenderAPI->DrawPointElement(VAO, pointsize, elementcount);
	}

	inline void RenderCommand::DrawTriangleElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount)
	{
		VAO->Bind();
		s_RenderAPI->DrawTriangleElement(VAO, elementcount);
	}

	inline void RenderCommand::DrawLineElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount, AQFLOAT width)
	{
		VAO->Bind();
		s_RenderAPI->DrawLineElement(VAO, elementcount, width);
	}

	inline void RenderCommand::DrawLineElement(const AQRef<AQVertexArray>& VAO, const std::vector<AQUINT>& elementcounts, AQFLOAT width)
	{
		VAO->Bind();
		s_RenderAPI->DrawLineElement(VAO, elementcounts, width);
	}

	inline void RenderCommand::SetLineWidth(const AQFLOAT width)
	{
		s_RenderAPI->SetLineWidth(width);
	}

}