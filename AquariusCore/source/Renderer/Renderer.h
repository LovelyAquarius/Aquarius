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
		static void OnWindowResize(AQUINT width, AQUINT height);
		static void BeginScene(const OrthgraphicCamera& camera);
		static void EndScene();
		
		static void Submit(const AQRef<AQVertexArray>& VAO, const AQRef<AQShader>& shader, const Eigen::Matrix4f& transform);


		static GraphicAPI& GetGraphicAPI() { return RenderAPI::GetGraphicAPI(); }

	private:
		//临时
		struct SceneData
		{
			Eigen::Matrix4f ViewProjection;
		};

		static SceneData* m_SceneData;

	};




}


