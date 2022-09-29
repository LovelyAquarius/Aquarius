#include "AQPCH.h"
#include "RenderCommand.h"
#include "Platform/AQOpenGL/OpenGLRenderAPI.h"

namespace Aquarius
{
	RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI();

	 void RenderCommand::SetClearcolor(const glm::vec4& color)
	{
		s_RenderAPI->SetClearcolor(color);
	}

	 void RenderCommand::Clear()
	{
		s_RenderAPI->Clear();
	}

	void RenderCommand::DrawElement(const std::shared_ptr<AQVertexArray>& VAO)
	{
		VAO->Bind();
		s_RenderAPI->DrawElement(VAO);
	}

	void RenderCommand::DrawElement(const std::shared_ptr<AQVertexArray>& VAO, uint32_t elementlocation)
	{
		VAO->Bind();
		s_RenderAPI->DrawElement(VAO, elementlocation);
	}






}