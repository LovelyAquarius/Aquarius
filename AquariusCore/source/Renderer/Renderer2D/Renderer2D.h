#pragma once
#include "Renderer/RenderCommand.h"
#include "Renderer/AQCamera.h"
#include "Renderer/EditorCamera.h"
#include "Renderer/OrthgraphicCamera.h"
#include "Renderer/AQBuffer.h"
#include "Renderer/AQFrameBuffer.h"
#include "Renderer/AQShader.h"
#include "Renderer/AQTexture.h"
#include "MathSystem/AQMathEx.h"

#include "Utils/AQGeometry/AQEuclidian2D.h"

#include "Renderer2DData.h"
#include "Scene/ElementSystem/AQComponents/AQSpiriteComponent.h"
namespace Aquarius
{
	//这是一个静态类，不需要实例，勿创建实例。
	class AQUARIUS_API Renderer2D
	{
	public:
		struct RenderCamera
		{
			Eigen::Matrix4f Projection;
			Eigen::Matrix4f View;
		};

	public:
		static void Init();
		static void ShutDown();
		static void BeginScene(AQCamera& camera,const Eigen::Matrix4f& transform);
		static void BeginScene(EditorCamera& camera);
		static void BeginScene(OrthgraphicCamera& camera);

		static void Flush();
		static void EndScene();

	public:
		//DrawQuad
		static void DrawQuad(const Eigen::Vector2f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const Eigen::Vector4f& color);
		static void DrawQuad(const Eigen::Vector3f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const Eigen::Vector4f& color);
		static void DrawQuad(const Eigen::Matrix4f& transform, const Eigen::Vector4f& color);
		static void DrawQuad(const Eigen::Vector3f& position, const Eigen::Vector3f& aboutpoint, const AQFLOAT  rotation, const Eigen::Vector2f& size, const Eigen::Vector4f& color);//绕某个点旋转版本
		static void DrawQuad(const Eigen::Vector2f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const AQRef<AQTexture2D>& texture, AQFLOAT tilingfactor = 1.0f);
		static void DrawQuad(const Eigen::Vector3f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const AQRef<AQTexture2D>& texture, AQFLOAT tilingfactor = 1.0f);
		static void DrawQuad(const Eigen::Vector3f& position, const Eigen::Vector3f& aboutpoint, const AQFLOAT  rotation, const Eigen::Vector2f& size, const AQRef<AQTexture2D>& texture, AQFLOAT tilingfactor = 1.0f);//绕某个点旋转版本
		static void DrawSpirite(const Eigen::Matrix4f& transform, const AQRef<AQSpiriteComponent> spirite, AQUINT entityid);
																																																					   
																																																					   //  //DrawBezier
		////TODO图元用的线段折线处会有缺口现象，需要解决
		//static void DrawBezier_Line_CPU(const Eigen::Vector3f& startpoint, const Eigen::Vector3f& endpoint, const Eigen::Vector3f& controlpoint, AQUINT count, const Eigen::Vector4f& color);//这个方法是在cpu上计算贝塞尔点，并采用线条着色器绘制。
		//static void DrawBezier_Line_CPU(const AQRef<AQQuadraticBezierCurve2D>& beziercurve, AQUINT count, const Eigen::Vector4f& color);
		//static void DrawBezier_Line_GPU(const Eigen::Vector3f& startpoint, const Eigen::Vector3f& endpoint, const Eigen::Vector3f& controlpoint, AQUINT count, const Eigen::Vector4f& color, const AQFLOAT width);//这个方法是在GPU上计算贝塞尔点，并采用专用的贝塞尔着色器绘制。
		//static void DrawBezier_Line_GPU(const AQRef<AQQuadraticBezierCurve2D>& beziercurve, AQUINT count, const Eigen::Vector4f& color, const AQFLOAT width);

	public:
		static Renderer2DData::Statistics GetStatistics();
		static void ResetStatistics();
		static Eigen::Vector2ui  GetRenderViewPort();
		static void DrawBuffer(const AQRef<AQFrameBuffer>& framebuffer);

	private:
		static void FlushAndReset();
		static void Begin_Batch() ;
		

	public:
		static AQBOOL IsDrawing();
	};
}
