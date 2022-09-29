#pragma once
#include "RenderCommand.h"
#include "OrthgraphicCamera.h"
#include "Platform/AQOpenGL/AQGLShader.h"

namespace Aquarius
{
	class AQUARIUS_API Renderer
	{
	public:
		static void BeginScene(OrthgraphicCamera& camera);
		static void EndScene();
		
		static void Submit(const std::shared_ptr<AQVertexArray>& VAO, const std::shared_ptr<AQGLShader>& shader);


		static GraphicAPI& GetGraphicAPI() { return RenderAPI::GetGraphicAPI(); }

	private:
		//临时
		struct SceneData
		{
			glm::mat4 ViewProjection;
		};

		static SceneData* m_SceneData;

	};




}


