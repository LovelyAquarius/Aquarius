#pragma once
#include "Renderer/AQTexture.h"
#include "Renderer/AQShader.h"
#include "Renderer/AQBuffer.h"
#include "MathSystem/AQMathEx.h"

namespace Aquarius
{
	struct QuadVertex2DBuffer
	{
		AQFLOAT Position[4];
		AQFLOAT Color[4];
		AQFLOAT TexCoord[2];
		AQFLOAT TexIndex;
		AQFLOAT TilingFactor;
		AQFLOAT entityid;//不知道为什么int值无法正确传入shader，后续有空再调试
	};



	class QuadVertex2DData :public AQObject
	{

	public:
		static AQRef<QuadVertex2DData> Create();
	public:
		bool IsDrawing = false;
		//Quad缓冲大小
		AQUINT MaxQuads;
		AQUINT MaxQuadVertices;
		AQUINT MaxQuadIndices;
		//Quad ABE
		AQRef<AQVertexArray> QuadVAO2D;
		AQRef<AQVertexBuffer> QuadVBO2D;
		AQRef<AQElementBuffer>QuadEBO2D;
		AQRef<AQShader> QuadShader;
		AQUINT QuadElementCount = 0;
		QuadVertex2DBuffer* QuadVertexBase = nullptr;
		QuadVertex2DBuffer* QuadVertexPointer = nullptr;
		Eigen::Vector4f QuadPosition[4];
		Eigen::Vector2f TexCoord[4];

	public:
		void SetShader(AQRef<AQShader>& shader);
		void SetBuffer(const Eigen::Matrix4f& transform, const Eigen::Vector4f& color, AQFLOAT textureindex, AQFLOAT tilingfactor, AQINT entityid);
		void ReSet(AQUINT maxquads);
		void ResetData();
		void SubmitData();
		//返回 DrawCall数量
		AQUINT DrawData();
	private: 

	protected:
		QuadVertex2DData(AQUINT maxquads = 20000);
		~QuadVertex2DData();
		QuadVertex2DData(const QuadVertex2DData& other) = delete;

	};


}




