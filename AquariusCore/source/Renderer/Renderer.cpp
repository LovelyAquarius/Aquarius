#include "AQPCH.h"
#include "Renderer.h"
#include "Renderer/Renderer2D.h"

namespace Aquarius
{
	Renderer::SceneData* Renderer::m_SceneData= new Renderer::SceneData();


	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(AQUINT width, AQUINT height)
	{
		//临时的，这会影响全局的渲染
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const OrthgraphicCamera& camera)
	{
		m_SceneData->ViewProjection = *(Eigen::Matrix4f*)&camera.GetViewProjection();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const AQRef<AQVertexArray>& VAO, const  AQRef<AQShader>& shader, const Eigen::Matrix4f& transform)
	{
		shader->Bind();
		shader->SetValue("u_VP", m_SceneData->ViewProjection);
		shader->SetValue("u_transform", transform);
	   
		VAO->Bind();
		RenderCommand::DrawTriangleElement(VAO,0);
	}


}