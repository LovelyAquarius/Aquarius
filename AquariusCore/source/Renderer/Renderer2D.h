#pragma once
#include "RenderCommand.h"
#include "AQCamera.h"
#include "OrthgraphicCamera.h"
#include "AQBuffer.h"
#include "AQShader.h"
#include "AQTexture.h"
#include "MathSystem/AQEigen.h"

#include "Utils/AQGeometry/AQEuclidian2D.h"

namespace Aquarius
{
	enum Renderer2D_On_Use
	{
		CommonTexture2D, QuadVertex2D, LineVertex2D, QudraticBezierUnit2D
	};





	//这是一个静态类，不需要实例，勿创建实例。
	class AQUARIUS_API Renderer2D
	{
	public:
		struct  Statistics
		{
			AQUINT DrawCalls = 0;

			AQUINT QuadCount = 0;
			AQUINT LineVertexCount = 0;
			AQUINT QuadraticBezierUnitCount = 0;

			AQUINT GetQuadVertexCount() { return QuadCount * 4; }
			AQUINT GetQuadElementCount() { return QuadCount * 6; }
			AQUINT GetBezierVertexCount() { return LineVertexCount; }
			AQUINT GetQudraticBezierUnitCount() { return QuadraticBezierUnitCount; }
		};

		struct RenderCamera
		{
			Eigen::Matrix4f Projection;
			Eigen::Matrix4f View;
		};

	public:
		static void Init();
		static void ShutDown();
		static void BeginScene(AQCamera& camera,const Eigen::Matrix4f& transform);
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
	   //DrawBezier
		//TODO图元用的线段折线处会有缺口现象，需要解决
		static void DrawBezier_Line_CPU(const Eigen::Vector3f& startpoint, const Eigen::Vector3f& endpoint, const Eigen::Vector3f& controlpoint, AQUINT count, const Eigen::Vector4f& color);//这个方法是在cpu上计算贝塞尔点，并采用线条着色器绘制。
		static void DrawBezier_Line_CPU(const AQRef<AQQuadraticBezierCurve2D>& beziercurve, AQUINT count, const Eigen::Vector4f& color);
		static void DrawBezier_Line_GPU(const Eigen::Vector3f& startpoint, const Eigen::Vector3f& endpoint, const Eigen::Vector3f& controlpoint, AQUINT count, const Eigen::Vector4f& color, const AQFLOAT width);//这个方法是在GPU上计算贝塞尔点，并采用专用的贝塞尔着色器绘制。
		static void DrawBezier_Line_GPU(const AQRef<AQQuadraticBezierCurve2D>& beziercurve, AQUINT count, const Eigen::Vector4f& color, const AQFLOAT width);

	public:
		static Statistics GetStatistics();
		static void ResetStatistics();

	private:
		static AQUINT MatchTextureIndex(const AQRef<AQTexture2D>& texture);
		static void SetQuadDataToVertexBuffer(const Eigen::Matrix4f& transform, const Eigen::Vector4f& color, AQFLOAT textureindex, AQFLOAT tilingfactor);
		static void FlushAndReset();
		static void Init_CommonTexture2D();//这个不能随意改动
		static void SubmitSubData(Renderer2D_On_Use subdata);
		static void DrawSubData(Renderer2D_On_Use subdata);
		static void ResetSubData(Renderer2D_On_Use subdata);
		static void PassCameraVPIntoShader(const Eigen::Matrix4f& camera_vp, Renderer2D_On_Use subdata);

	public:
		static void ShutDown(Renderer2D_On_Use subdata);
		static void Init_QuadVertex2D(AQUINT maxquads = 20000);
		static void Init_LineVertex2D (AQUINT maxlinevertices = 20000);
		static void Init_QudraticBezierUnit2D(AQUINT maxquadraticbezierunits = 2000);
		
	};
}
//Render2D相关数据结构
namespace Aquarius
{
	//通用纹理数据 勿随意改动
	struct CommonTexture2DData
	{
		//最大纹理槽数量，取决于实现
		static const AQUINT MaxTextureSlots = 32;//static?
		//纹理槽数量
		std::array<AQRef<AQTexture2D>, MaxTextureSlots> TextureSlots;
		AQUINT TextureSlotIndex = 1;

		//PixelTexture数据
		AQRef<AQTexture2D> DefaultTexture2D;
		AQRef<AQShader> DefaultTextureShader2D;
	};

	//Quad数据  对应三角形图元
	struct QuadVertex2DBuffer
	{
		Eigen::Vector4f Position;
		Eigen::Vector4f Color;
		Eigen::Vector2f TexCoord;
		AQFLOAT TexIndex;
		AQFLOAT TilingFactor;
	};
	struct QuadVertex2DData
	{
		bool IsDrawing = false;
		//Quad缓冲大小
		const AQUINT MaxQuads;
		const AQUINT MaxQuadVertices;
		const AQUINT MaxQuadIndices;
		//Quad ABE
		AQRef<AQVertexArray> QuadVAO2D;
		AQRef<AQVertexBuffer> QuadVBO2D;
		AQRef<AQElementBuffer>QuadEBO2D;
		AQRef<AQShader> TextureShader2D;
		AQUINT QuadElementCount = 0;
		QuadVertex2DBuffer* QuadVertexBase = nullptr;//不知道用智能指针是否会影响渲染的性能？
		QuadVertex2DBuffer* QuadVertexPointer = nullptr;
		Eigen::Vector4f QuadPosition[4];

		QuadVertex2DData(AQUINT maxquads = 20000) :MaxQuads(maxquads),
			MaxQuadVertices(maxquads * 4), MaxQuadIndices(maxquads * 6) {}
		~QuadVertex2DData() { if(QuadVertexBase) delete[] QuadVertexBase; }
	};

	//Line数据  对应连续连线的图元
	struct LineVertex2DBuffer
	{
		Eigen::Vector3f Position;
		Eigen::Vector4f Color;
	};
	struct LineVertex2DData
	{
		bool IsDrawing = false;
		//Line缓冲大小
		const AQUINT MaxLineVertices;
		const AQUINT MaxLineIndices;

		AQRef<AQVertexArray> LineVAO2D;
		AQRef<AQVertexBuffer> LineVBO2D;
		AQRef<AQElementBuffer>LineEBO2D;
		AQRef<AQShader> LineShader2D;
		std::vector<AQUINT> LineElementCounts;
		LineVertex2DBuffer* LineVertexBase = nullptr;
		LineVertex2DBuffer* LineVertexPointer = nullptr;

		LineVertex2DData(AQUINT maxlinevertices = 20000) :MaxLineVertices(maxlinevertices) , MaxLineIndices(maxlinevertices) {};
		~LineVertex2DData() { if(LineVertexBase) delete[] LineVertexBase; }
	};

	//QuadraticBezier数据  对应点图元
	struct QudraticBezierUnit2DBuffer
	{
		Eigen::Vector3f start;
		Eigen::Vector3f end;
		Eigen::Vector3f control;
		AQFLOAT vertexcount;
		Eigen::Vector4f Color;
	};
	struct QudraticBezierUnit2DData
	{
		bool IsDrawing = false;
		//QuadraticBezier缓冲大小
		const AQUINT MaxQuadraticBezierUnits = 2000;
		const AQUINT MaxQuadraticBezierUnitIndices = MaxQuadraticBezierUnits;

		AQRef<AQVertexArray> QuadraticBezierUnitVAO2D;
		AQRef<AQVertexBuffer> QuadraticBezierUnitVBO2D;
		AQRef<AQElementBuffer>QuadraticBezierUnitEBO2D;
		AQRef<AQShader> QuadraticBezierUnitShader2D;
		std::vector<AQFLOAT> QuadraticBezierUnitLineWidth;
		std::vector<AQUINT> QuadraticBezierUnitElementCounts;
		QudraticBezierUnit2DBuffer* QuadraticBezierUnitBase = nullptr;
		QudraticBezierUnit2DBuffer* QuadraticBezierUnitPointer = nullptr;

		QudraticBezierUnit2DData(AQUINT maxquadraticbezierunits = 2000) :MaxQuadraticBezierUnits(maxquadraticbezierunits), MaxQuadraticBezierUnitIndices(maxquadraticbezierunits) {}
		~QudraticBezierUnit2DData(){ if (QuadraticBezierUnitBase) delete[] QuadraticBezierUnitBase; }
	};

	//这些子数据后面要改成entity系统！
	//这个数据不可以自定义更改！会导致程序直接崩溃的风险！
	struct  Renderer2DData
	{
		//子数据指针
		std::unique_ptr<CommonTexture2DData> CommonTexture2D = nullptr;
		std::unique_ptr<QuadVertex2DData> QuadVertex2D=nullptr;
		std::unique_ptr<LineVertex2DData> LineVertex2D = nullptr;
		std::unique_ptr<QudraticBezierUnit2DData> QudraticBezierUnit2D = nullptr;

		//渲染器状态信息
		std::vector<Renderer2D_On_Use> OnUses;
		Renderer2D::Statistics Statistics;
	};





}