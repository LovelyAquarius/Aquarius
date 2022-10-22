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
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(const OrthgraphicCamera& camera);
		static void EndScene();
		
		static void Submit(const AQRef<AQVertexArray>& VAO, const AQRef<AQShader>& shader, const glm::mat4& transform =  glm::mat4(1.0f) );


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


