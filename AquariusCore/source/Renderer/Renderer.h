#pragma once
#include "RenderCommand.h"
#include "OrthgraphicCamera.h"
#include "AQShader.h"

namespace Aquarius
{
	class AQUARIUS_API Renderer
	{
	public:
		static void Init();
		static void BeginScene(OrthgraphicCamera& camera);
		static void EndScene();
		
		static void Submit( AQRef<AQVertexArray>& VAO,  AQRef<AQShader>& shader, const glm::mat4& transform =  glm::mat4(1.0f) );


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


