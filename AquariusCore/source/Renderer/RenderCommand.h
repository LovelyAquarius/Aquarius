#pragma once
#include "RenderAPI.h"


namespace Aquarius
{

	class AQUARIUS_API RenderCommand
	{
	public:

		inline static void Init();
		inline static void SetClearcolor(const glm::vec4& color);
		inline static void Clear();
		inline static void DrawElement(const AQRef<AQVertexArray>& VAO);
		inline static void DrawElement(const AQRef<AQVertexArray>& VAO, uint32_t elementlocation);
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

	inline void RenderCommand::SetClearcolor(const glm::vec4& color)
	{
		s_RenderAPI->SetClearcolor(color);
	}

	inline void RenderCommand::Clear()
	{
		s_RenderAPI->Clear();
	}

	inline void RenderCommand::DrawElement(const AQRef<AQVertexArray>& VAO)
	{
		VAO->Bind();
		s_RenderAPI->DrawElement(VAO);
	}

	inline void RenderCommand::DrawElement(const AQRef<AQVertexArray>& VAO, uint32_t elementlocation)
	{
		VAO->Bind();
		s_RenderAPI->DrawElement(VAO, elementlocation);
	}


}