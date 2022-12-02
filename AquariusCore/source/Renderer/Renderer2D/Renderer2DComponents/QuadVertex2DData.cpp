#include "AQPCH.h"
#include "QuadVertex2DData.h"
#include "Renderer/RenderCommand.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"
#include "Renderer/Renderer2D/Renderer2DData.h"
#include "CommonTexture2DData.h"

#pragma warning(disable:4244)

namespace Aquarius
{


	AQRef<QuadVertex2DData> QuadVertex2DData::Create()
	{
		return new QuadVertex2DData();
	}

	void QuadVertex2DData::SetShader(AQRef<AQShader>& shader)
	{
		if(shader)
			QuadShader = shader;
	}

	void QuadVertex2DData::SetBuffer(const Eigen::Matrix4f& transform, const Eigen::Vector4f& color, AQFLOAT textureindex, AQFLOAT tilingfactor, AQINT entityid)
	{
		for (int i = 0; i < 4; i++)
		{
			auto pos = transform * QuadPosition[i];
			QuadVertexPointer->Position[0] = pos.x();
			QuadVertexPointer->Position[1] = pos.y();
			QuadVertexPointer->Position[2] = pos.z();
			QuadVertexPointer->Position[3] = pos.w();
			QuadVertexPointer->Color[0] = color.x();
			QuadVertexPointer->Color[1] = color.y();
			QuadVertexPointer->Color[2] = color.z();
			QuadVertexPointer->Color[3] = color.w();
			QuadVertexPointer->TexCoord[0] = TexCoord[i].x();
			QuadVertexPointer->TexCoord[1] = TexCoord[i].y();
			QuadVertexPointer->TexIndex = textureindex;
			QuadVertexPointer->TilingFactor = tilingfactor;
			QuadVertexPointer->entityid = (AQFLOAT)entityid;
			QuadVertexPointer++;
		}
		QuadElementCount += 6;
	}

	void QuadVertex2DData::ReSet(AQUINT maxquads)
	{
		if (IsDrawing)
		{
			AQ_CORE_WARN("QuadVertex2DData::ReSet: The data of QuadVertex2D has been using now, its not allowed to delete the data!");
			return;
		}
		if (!maxquads)
			return;
		if (QuadVertexBase) delete[] QuadVertexBase;
		MaxQuads = maxquads;
		MaxQuadVertices = maxquads * 4;
		MaxQuadIndices = maxquads * 6;

		QuadPosition[0] = Eigen::Vector4f{ -0.5f,-0.5f,0.0f,1.0f };
		QuadPosition[1] = Eigen::Vector4f{ 0.5f,-0.5f,0.0f,1.0f };
		QuadPosition[2] = Eigen::Vector4f{ 0.5f,0.5f,0.0f,1.0f };
		QuadPosition[3] = Eigen::Vector4f{ -0.5f,0.5f,0.0f,1.0f };
		Eigen::Vector2f texcoord;
		texcoord << 0.0f, 0.0f;
		TexCoord[0] = texcoord;
		texcoord << 1.0f, 0.0f;
		TexCoord[1] = texcoord;
		texcoord << 1.0f, 1.0f;
		TexCoord[2] = texcoord;
		texcoord << 0.0f, 1.0f;
		TexCoord[3] = texcoord;
		QuadVAO2D = AQVertexArray::Create("quad2d");
		QuadVertexBase = new QuadVertex2DBuffer[MaxQuadVertices];
		QuadVBO2D = AQVertexBuffer::Create(MaxQuadVertices * sizeof(QuadVertex2DBuffer), "s_Data2D->QuadVBO2D");
		QuadVBO2D->SetLayout({
			{ "vertex",0,BufferDataType::Float4 },
			{ "color",1,BufferDataType::Float4 },
			{ "texcoord",2,BufferDataType::Float2 },
			{ "texindex",3,BufferDataType::Float1 } ,
			{ "tilingfactor",4,BufferDataType::Float1 },
			{ "entityid",5,BufferDataType::Float1 } });

		AQUINT* quad2dindices = new AQUINT[MaxQuadIndices];
		AQUINT offset = 0;
		for (AQUINT i = 0; i < MaxQuadIndices; i += 6)
		{
			quad2dindices[i + 0] = offset + 0;
			quad2dindices[i + 1] = offset + 1;
			quad2dindices[i + 2] = offset + 2;
			quad2dindices[i + 3] = offset + 2;
			quad2dindices[i + 4] = offset + 3;
			quad2dindices[i + 5] = offset + 0;
			offset += 4;
		}

		QuadEBO2D = AQElementBuffer::Create(MaxQuadIndices, quad2dindices, GL_DYNAMIC_DRAW, "QuadEBO2D");
		delete[] quad2dindices;
		AQRefCast<AQGLVertexArray>(QuadVAO2D)->LinkVE(QuadVBO2D, QuadEBO2D);
		AQRefCast<AQGLVertexArray>(QuadVAO2D)->Enable(0, 1, 2, 3, 4, 5);
	}

	void QuadVertex2DData::ResetData()
	{
		QuadElementCount = 0;
		QuadVertexPointer = QuadVertexBase;
	}

	void QuadVertex2DData::SubmitData()
	{
		AQUINT quadsize = (uint8_t*)QuadVertexPointer - (uint8_t*)QuadVertexBase;
		QuadVBO2D->SetData(QuadVertexBase, quadsize);
	}

	AQUINT QuadVertex2DData::DrawData()
	{
		if (!IsDrawing)
			return 0;

		QuadShader->Bind();
		RenderCommand::DrawTriangleElement(QuadVAO2D, QuadElementCount);
		IsDrawing = false;
		return 1;
	}



	QuadVertex2DData::QuadVertex2DData(AQUINT maxquads)
		:MaxQuads(maxquads),MaxQuadVertices(maxquads * 4), MaxQuadIndices(maxquads * 6) 
	{
		if (!Renderer2DData::s_Renderer2DData)
			AQ_ASSERT(false, "QuadVertex2DData::QuadVertex2DData: Creating QuadVertex2DData Component failed, becuase the Renderer2DData has not been initialized yet!");
		if (!Renderer2DData::s_Renderer2DData->HasEntity<CommonTexture2DData>())
			AQ_ASSERT(false, "QuadVertex2DData::QuadVertex2DData: Creating QuadVertex2DData Component failed, becuase the CommonTexture2DData Component has not been initialized yet!");

		AQ_INITIALIZE_AQOBJECT_TYPE(QuadVertex2DData);

		QuadPosition[0] = Eigen::Vector4f{ -0.5f,-0.5f,0.0f,1.0f };
		QuadPosition[1] = Eigen::Vector4f{ 0.5f,-0.5f,0.0f,1.0f };
		QuadPosition[2] = Eigen::Vector4f{ 0.5f,0.5f,0.0f,1.0f };
		QuadPosition[3] = Eigen::Vector4f{ -0.5f,0.5f,0.0f,1.0f };
		Eigen::Vector2f texcoord;
		texcoord << 0.0f, 0.0f;
		TexCoord[0] = texcoord;
		texcoord << 1.0f, 0.0f;
		TexCoord[1] = texcoord;
		texcoord << 1.0f, 1.0f;
		TexCoord[2] = texcoord;
		texcoord << 0.0f, 1.0f;
		TexCoord[3] = texcoord;
		QuadVAO2D = AQVertexArray::Create("quad2d");
		QuadVertexBase = new QuadVertex2DBuffer[MaxQuadVertices];
		QuadVBO2D = AQVertexBuffer::Create(MaxQuadVertices * sizeof(QuadVertex2DBuffer), "s_Data2D->QuadVBO2D");
		QuadVBO2D->SetLayout({
			{ "vertex",0,BufferDataType::Float4 },
			{ "color",1,BufferDataType::Float4 },
			{ "texcoord",2,BufferDataType::Float2 },
			{ "texindex",3,BufferDataType::Float1 } ,
			{ "tilingfactor",4,BufferDataType::Float1 },
			{ "entityid",5,BufferDataType::Float1 } });

		AQUINT* quad2dindices = new AQUINT[MaxQuadIndices];
		AQUINT offset = 0;
		for (AQUINT i = 0; i < MaxQuadIndices; i += 6)
		{
			quad2dindices[i + 0] = offset + 0;
			quad2dindices[i + 1] = offset + 1;
			quad2dindices[i + 2] = offset + 2;
			quad2dindices[i + 3] = offset + 2;
			quad2dindices[i + 4] = offset + 3;
			quad2dindices[i + 5] = offset + 0;
			offset += 4;
		}

		QuadEBO2D = AQElementBuffer::Create(MaxQuadIndices, quad2dindices, GL_DYNAMIC_DRAW, "QuadEBO2D");
		delete[] quad2dindices;
		AQRefCast<AQGLVertexArray>(QuadVAO2D)->LinkVE(QuadVBO2D, QuadEBO2D);
		AQRefCast<AQGLVertexArray>(QuadVAO2D)->Enable(0, 1, 2, 3, 4, 5);
		QuadShader = Renderer2DData::s_Renderer2DData->GetEntityComponent<CommonTexture2DData>()->DefaultTextureShader2D;
	}
	QuadVertex2DData::~QuadVertex2DData()
	{
		if (QuadVertexBase) delete[] QuadVertexBase;
	}







}