#pragma once
#include "RenderCommand.h"
#include "OrthgraphicCamera.h"
#include "AQBuffer.h"
#include "AQShader.h"
#include "AQTexture.h"
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
			uint32_t DrawCalls = 0;

			uint32_t QuadCount = 0;
			uint32_t LineVertexCount = 0;
			uint32_t QuadraticBezierUnitCount = 0;

			uint32_t GetQuadVertexCount() { return QuadCount * 4; }
			uint32_t GetQuadElementCount() { return QuadCount * 6; }
			uint32_t GetBezierVertexCount() { return LineVertexCount; }
			uint32_t GetQudraticBezierUnitCount() { return QuadraticBezierUnitCount; }
		};

	public:
		static void Init();
		static void ShutDown();
		static void BeginScene(OrthgraphicCamera& camera);
		static void Flush();
		static void EndScene();

	public:
		//DrawQuad
		static void DrawQuad(const glm::vec2& position, const float  rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float  rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec3& aboutpoint, const float  rotation, const glm::vec2& size, const glm::vec4& color);//绕某个点旋转版本
		static void DrawQuad(const glm::vec2& position, const float  rotation, const glm::vec2& size, const AQRef<AQTexture2D>& texture, float tilingfactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const float  rotation, const glm::vec2& size, const AQRef<AQTexture2D>& texture, float tilingfactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec3& aboutpoint, const float  rotation, const glm::vec2& size, const AQRef<AQTexture2D>& texture, float tilingfactor = 1.0f);//绕某个点旋转版本
	   //DrawBezier
		//TODO图元用的线段折线处会有缺口现象，需要解决
		static void DrawBezier_Line_CPU(const glm::vec3& startpoint, const glm::vec3& endpoint, const glm::vec3& controlpoint, uint32_t count, const glm::vec4& color);//这个方法是在cpu上计算贝塞尔点，并采用线条着色器绘制。
		static void DrawBezier_Line_CPU(const AQRef<AQQuadraticBezierCurve2D>& beziercurve, uint32_t count, const glm::vec4& color);
		static void DrawBezier_Line_GPU(const glm::vec3& startpoint, const glm::vec3& endpoint, const glm::vec3& controlpoint, uint32_t count, const glm::vec4& color, const float width);//这个方法是在GPU上计算贝塞尔点，并采用专用的贝塞尔着色器绘制。
		static void DrawBezier_Line_GPU(const AQRef<AQQuadraticBezierCurve2D>& beziercurve, uint32_t count, const glm::vec4& color, const float width);

	public:
		static Statistics GetStatistics();
		static void ResetStatistics();

	private:
		static uint32_t MatchTextureIndex(const AQRef<AQTexture2D>& texture);
		static void SetQuadDataToVertexBuffer(const glm::mat4& transform, const glm::vec4& color, float textureindex, float tilingfactor);
		static void FlushAndReset();
		static void Init_CommonTexture2D();//这个不能随意改动
		static void SubmitSubData(Renderer2D_On_Use subdata);
		static void DrawSubData(Renderer2D_On_Use subdata);
		static void ResetSubData(Renderer2D_On_Use subdata);
		static void PassCameraVPIntoShader(OrthgraphicCamera& camera, Renderer2D_On_Use subdata);

	public:
		static void ShutDown(Renderer2D_On_Use subdata);
		static void Init_QuadVertex2D(uint32_t maxquads = 20000);
		static void Init_LineVertex2D (uint32_t maxlinevertices = 20000);
		static void Init_QudraticBezierUnit2D(uint32_t maxquadraticbezierunits = 2000);
		
	};
}
//Render2D相关数据结构
namespace Aquarius
{
	//通用纹理数据 勿随意改动
	struct CommonTexture2DData
	{
		//最大纹理槽数量，取决于实现
		static const uint32_t MaxTextureSlots = 32;//static?
		//纹理槽数量
		std::array<AQRef<AQTexture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		//PixelTexture数据
		AQRef<AQTexture2D> DefaultTexture2D;
		AQRef<AQShader> DefaultTextureShader2D;
	};

	//Quad数据  对应三角形图元
	struct QuadVertex2DBuffer
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};
	struct QuadVertex2DData
	{
		bool IsDrawing = false;
		//Quad缓冲大小
		const uint32_t MaxQuads;
		const uint32_t MaxQuadVertices;
		const uint32_t MaxQuadIndices;
		//Quad ABE
		AQRef<AQVertexArray> QuadVAO2D;
		AQRef<AQVertexBuffer> QuadVBO2D;
		AQRef<AQElementBuffer>QuadEBO2D;
		AQRef<AQShader> TextureShader2D;
		uint32_t QuadElementCount = 0;
		QuadVertex2DBuffer* QuadVertexBase = nullptr;//不知道用智能指针是否会影响渲染的性能？
		QuadVertex2DBuffer* QuadVertexPointer = nullptr;
		glm::vec4 QuadPosition[4];

		QuadVertex2DData(uint32_t maxquads = 20000) :MaxQuads(maxquads),
			MaxQuadVertices(maxquads * 4), MaxQuadIndices(maxquads * 6) {}
		~QuadVertex2DData() { if(QuadVertexBase) delete[] QuadVertexBase; }
	};

	//Line数据  对应连续连线的图元
	struct LineVertex2DBuffer
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};
	struct LineVertex2DData
	{
		bool IsDrawing = false;
		//Line缓冲大小
		const uint32_t MaxLineVertices;
		const uint32_t MaxLineIndices;

		AQRef<AQVertexArray> LineVAO2D;
		AQRef<AQVertexBuffer> LineVBO2D;
		AQRef<AQElementBuffer>LineEBO2D;
		AQRef<AQShader> LineShader2D;
		std::vector<uint32_t> LineElementCounts;
		LineVertex2DBuffer* LineVertexBase = nullptr;
		LineVertex2DBuffer* LineVertexPointer = nullptr;

		LineVertex2DData(uint32_t maxlinevertices = 20000) :MaxLineVertices(maxlinevertices) , MaxLineIndices(maxlinevertices) {};
		~LineVertex2DData() { if(LineVertexBase) delete[] LineVertexBase; }
	};

	//QuadraticBezier数据  对应点图元
	struct QudraticBezierUnit2DBuffer
	{
		glm::vec3 start;
		glm::vec3 end;
		glm::vec3 control;
		float vertexcount;
		glm::vec4 Color;
	};
	struct QudraticBezierUnit2DData
	{
		bool IsDrawing = false;
		//QuadraticBezier缓冲大小
		const uint32_t MaxQuadraticBezierUnits = 2000;
		const uint32_t MaxQuadraticBezierUnitIndices = MaxQuadraticBezierUnits;

		AQRef<AQVertexArray> QuadraticBezierUnitVAO2D;
		AQRef<AQVertexBuffer> QuadraticBezierUnitVBO2D;
		AQRef<AQElementBuffer>QuadraticBezierUnitEBO2D;
		AQRef<AQShader> QuadraticBezierUnitShader2D;
		std::vector<float> QuadraticBezierUnitLineWidth;
		std::vector<uint32_t> QuadraticBezierUnitElementCounts;
		QudraticBezierUnit2DBuffer* QuadraticBezierUnitBase = nullptr;
		QudraticBezierUnit2DBuffer* QuadraticBezierUnitPointer = nullptr;

		QudraticBezierUnit2DData(uint32_t maxquadraticbezierunits = 2000) :MaxQuadraticBezierUnits(maxquadraticbezierunits), MaxQuadraticBezierUnitIndices(maxquadraticbezierunits) {}
		~QudraticBezierUnit2DData(){ if (QuadraticBezierUnitBase) delete[] QuadraticBezierUnitBase; }
	};


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