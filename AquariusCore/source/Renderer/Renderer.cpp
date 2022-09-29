#include "AQPCH.h"
#include "Renderer.h"


namespace Aquarius
{
	Renderer::SceneData* Renderer::m_SceneData= new Renderer::SceneData();








	void Renderer::BeginScene(OrthgraphicCamera& camera)
	{
		m_SceneData->ViewProjection = camera.GetViewProjection();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<AQVertexArray>& VAO, const std::shared_ptr<AQGLShader>& shader)
	{
		shader->Use();
		shader->SetUniformVar("u_VP", m_SceneData->ViewProjection);

		VAO->Bind();
		RenderCommand::DrawElement(VAO);
	}


}