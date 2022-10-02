#include "AQPCH.h"
#include "Renderer.h"
#include"Platform/AQOpenGL/AQGLShader.h"

namespace Aquarius
{
	Renderer::SceneData* Renderer::m_SceneData= new Renderer::SceneData();


	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthgraphicCamera& camera)
	{
		m_SceneData->ViewProjection = camera.GetViewProjection();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit( AQRef<AQVertexArray>& VAO,   AQRef<AQShader>& shader, const glm::mat4& transform)
	{
		//GLspecific
	    AQRef<AQGLShader> specificshader=dynamic_cast< AQGLShader*>(( AQShader*)shader);
		specificshader->Bind();
		specificshader->SetUniformVar("u_VP", m_SceneData->ViewProjection);
		specificshader->SetUniformVar("u_VP", m_SceneData->ViewProjection);
		specificshader->SetUniformVar("u_transform", transform);

		VAO->Bind();
		RenderCommand::DrawElement(VAO);
	}


}