#include "AQPCH.h"
#include "Renderer2D.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"
#include "Platform/AQOpenGL/AQGLTexture.h"
#include "Renderer/Renderer2D/Renderer2DData.h"

namespace Aquarius
{
	extern Eigen::Vector2ui GlobalViewPortSize;
	void Renderer2D::Init()
	{
		//创建默认数据
		Renderer2DData::s_Renderer2DData = Renderer2DData::CreateData();
		Renderer2DData::s_Renderer2DData->CreateEntity<CommonTexture2DData>();
		Renderer2DData::s_Renderer2DData->CreateEntity<QuadVertex2DData>();
		Renderer2DData::s_Renderer2DData->CreateEntity<CommonCamera2DData>();
		//________________________________________________
		//创建自定义数据
		//_______________________________________________________
	}

	void Renderer2D::ShutDown()
	{
		if (IsDrawing())
		{
			AQ_CORE_WARN("Renderer2D::ShutDown: Failed to shutdown,SomeData is using now!");
			return;
		}
		delete Renderer2DData::s_Renderer2DData;
	}

	void Renderer2D::BeginScene(OrthgraphicCamera& camera)
	{
		auto subdata = Renderer2DData::s_Renderer2DData->GetEntityComponent<CommonCamera2DData>();
		subdata->SetData(camera.GetViewProjection());
		Begin_Batch();
	}

	void Renderer2D::BeginScene(AQCamera& camera, const Eigen::Matrix4f& transform)
	{
		Eigen::Matrix4f VP = camera.GetProjection() * transform;
		auto cameradata = Renderer2DData::s_Renderer2DData->GetEntityComponent<CommonCamera2DData>();
		cameradata->SetData(VP);
		Begin_Batch();
	}

	void Renderer2D::BeginScene(EditorCamera& camera)
	{
		
		Eigen::Matrix4f VP = camera.GetViewProjection();
		auto cameradata = Renderer2DData::s_Renderer2DData->GetEntityComponent<CommonCamera2DData>();
		cameradata->SetData(VP);
		Begin_Batch();

	}

	void Renderer2D::Flush()
	{
		{
			auto subdata = Renderer2DData::s_Renderer2DData->GetEntityComponent<CommonTexture2DData>();
			//绑定可用纹理槽
			for (AQUINT i = 0; i < subdata->TextureSlotIndex; i++)
				subdata->TextureSlots[i]->BindSlot(i);
		}
		//DrawCall
		{
			auto subdata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
			Renderer2DData::s_Renderer2DData->m_Statistics.DrawCalls += subdata->DrawData();
		}

	}
	void Renderer2D::FlushAndReset()
	{
		EndScene();

		{
			auto subdata = Renderer2DData::s_Renderer2DData->GetEntityComponent<CommonTexture2DData>();
			subdata->ResetData();
		}
		{
			auto subdata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
			subdata->ResetData();
		}
	}

	void Renderer2D::Begin_Batch()
	{
		{
			auto subdata = Renderer2DData::s_Renderer2DData->GetEntityComponent<CommonTexture2DData>();
			subdata->ResetData();
		}
		{
			auto subdata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
			subdata->ResetData();
		}
	}

	void Renderer2D::ResetStatistics() { memset(&(Renderer2DData::s_Renderer2DData->m_Statistics), 0, sizeof(Renderer2DData::s_Renderer2DData->m_Statistics)); }

	Eigen::Vector2ui Renderer2D::GetRenderViewPort()
	{
		return GlobalViewPortSize;
	}
	
	void Renderer2D::DrawBuffer(const AQRef<AQFrameBuffer>& framebuffer)
	{
		RenderCommand::DrawBuffer(framebuffer);
		Renderer2DData::s_Renderer2DData->m_Statistics.DrawCalls++;
	}
	Renderer2DData::Statistics Renderer2D::GetStatistics() { return Renderer2DData::s_Renderer2DData->m_Statistics; }

	void Renderer2D::EndScene()
	{
		{
			auto subdata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
			subdata->SubmitData();
		}
		Flush();
	}
	AQBOOL Renderer2D::IsDrawing()
	{
		{
			auto subdata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
			if (subdata->IsDrawing)
				return true;
		}
		return false;
	}
}
namespace Aquarius
{
	//DrawQuad___________________________________________________________________
	void Renderer2D::DrawQuad(const Eigen::Vector2f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const Eigen::Vector4f& color)
	{
		
		DrawQuad(Eigen::Vector3f(position.x(), position.y(), 0.0f), rotation, size, color);
	}

	void Renderer2D::DrawQuad(const Eigen::Vector3f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const Eigen::Vector4f& color)
	{

		const float textureindex = 0.0f;//这是纯白像素！
		constexpr float tilingfactor = 1.0f;

		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		transform.setIdentity();
		AQ_Scale_Self(transform.matrix(), Eigen::Vector3f(size.x(), size.y(), 1.0f));
		transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(rotation), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));
		transform.translate(position);

		auto quaddata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
		//检查Quad缓冲区是否已满
		if (quaddata ->QuadElementCount >= quaddata->MaxQuadIndices - 6)
			FlushAndReset();
		//______________________________________________________
		quaddata->SetBuffer(transform.matrix(), color, textureindex, tilingfactor,-1);
		Renderer2DData::s_Renderer2DData->m_Statistics.QuadCount++;
		quaddata->IsDrawing = true;
	}

	void Renderer2D::DrawQuad(const Eigen::Matrix4f& transform, const Eigen::Vector4f& color)
	{
		const float textureindex = 0.0f;//这是纯白像素！
		constexpr float tilingfactor = 1.0f;
		auto quaddata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
		//检查Quad缓冲区是否已满
		if (quaddata->QuadElementCount >= quaddata->MaxQuadIndices - 6)
			FlushAndReset();
		//______________________________________________________
		quaddata->SetBuffer(transform.matrix(), color, textureindex, tilingfactor,-1);
		Renderer2DData::s_Renderer2DData->m_Statistics.QuadCount++;
		quaddata->IsDrawing = true;
	}

	void Renderer2D::DrawQuad(const Eigen::Vector3f& position, const Eigen::Vector3f& aboutpoint, const AQFLOAT  rotation, const Eigen::Vector2f& size, const Eigen::Vector4f& color)
	{
		const float textureindex = 0.0f;//这是纯白像素！
		constexpr float tilingfactor = 10.0f;

		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		transform.setIdentity();
		AQ_Scale_Self(transform.matrix(), Eigen::Vector3f(size.x(), size.y(), 1.0f));
		transform.translate(-aboutpoint);
		transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(rotation), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));
		transform.translate(position + aboutpoint);

		auto quaddata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
		//检查Quad缓冲区是否已满
		if (quaddata->QuadElementCount >= quaddata->MaxQuadIndices - 6)
			FlushAndReset();
		//______________________________________________________
		quaddata->SetBuffer(transform.matrix(), color, textureindex, tilingfactor,-1);
		Renderer2DData::s_Renderer2DData->m_Statistics.QuadCount++;
		quaddata->IsDrawing = true;
	}


	void Renderer2D::DrawQuad(const Eigen::Vector2f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const AQRef<AQTexture2D>& texture, AQFLOAT tilingfactor)
	{
		DrawQuad(Eigen::Vector3f(position.x(), position.y(), 0.0f), rotation, size, texture, tilingfactor);
	}

	void Renderer2D::DrawQuad(const Eigen::Vector3f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const AQRef<AQTexture2D>& texture, AQFLOAT tilingfactor)
	{
		const Eigen::Vector4f color{ 1.0f,1.0f,1.0f,1.0f };

		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		transform.setIdentity();
		AQ_Scale_Self(transform.matrix(), Eigen::Vector3f(size.x(), size.y(), 1.0f));
		transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(rotation), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));
		transform.translate(position);

		auto commontexturecomponent = Renderer2DData::s_Renderer2DData->GetEntityComponent<CommonTexture2DData>();
		AQUINT textureindex = commontexturecomponent->FindTexture(texture);
		if (!textureindex)
		{
			textureindex = commontexturecomponent->TextureSlotIndex;
			commontexturecomponent->TextureSlots[commontexturecomponent->TextureSlotIndex] = texture;
			commontexturecomponent->TextureSlotIndex++;
		}

		auto quaddata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
		//检查Quad缓冲区是否已满
		if (quaddata->QuadElementCount >= quaddata->MaxQuadIndices - 6)
			FlushAndReset();
		//______________________________________________________
		quaddata->SetBuffer(transform.matrix(), color, (float)textureindex, tilingfactor,-1);
		Renderer2DData::s_Renderer2DData->m_Statistics.QuadCount++;
		quaddata->IsDrawing = true;
	}

	void Renderer2D::DrawQuad(const Eigen::Vector3f& position, const Eigen::Vector3f& aboutpoint, const AQFLOAT  rotation, const Eigen::Vector2f& size, const AQRef<AQTexture2D>& texture, AQFLOAT tilingfactor)
	{
		const Eigen::Vector4f color{ 1.0f,1.0f,1.0f,1.0f };

		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		transform.setIdentity();
		AQ_Scale_Self(transform.matrix(), Eigen::Vector3f(size.x(), size.y(), 1.0f));
		transform.translate(position);
		transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(rotation), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));


		auto commontexturecomponent = Renderer2DData::s_Renderer2DData->GetEntityComponent<CommonTexture2DData>();
		AQUINT textureindex = commontexturecomponent->FindTexture(texture);
		if (!textureindex)
		{
			textureindex = commontexturecomponent->TextureSlotIndex;
			commontexturecomponent->TextureSlots[commontexturecomponent->TextureSlotIndex] = texture;
			commontexturecomponent->TextureSlotIndex++;
		}

		auto quaddata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
		//检查Quad缓冲区是否已满
		if (quaddata->QuadElementCount >= quaddata->MaxQuadIndices - 6)
			FlushAndReset();
		//______________________________________________________
		quaddata->SetBuffer(transform.matrix(), color, (float)textureindex, tilingfactor,-1);
		Renderer2DData::s_Renderer2DData->m_Statistics.QuadCount++;
		quaddata->IsDrawing = true;
	}
	void Renderer2D::DrawSpirite(const Eigen::Matrix4f& transform, const AQRef<AQSpiriteComponent> spirite, AQUINT entityid)
	{
		const float textureindex = 0.0f;//这是纯白像素！
		constexpr float tilingfactor = 1.0f;
		auto quaddata = Renderer2DData::s_Renderer2DData->GetEntityComponent<QuadVertex2DData>();
		//检查Quad缓冲区是否已满
		if (quaddata->QuadElementCount >= quaddata->MaxQuadIndices - 6)
			FlushAndReset();
		//______________________________________________________
		quaddata->SetBuffer(transform.matrix(), spirite->Color, textureindex, tilingfactor, (AQINT)entityid);
		Renderer2DData::s_Renderer2DData->m_Statistics.QuadCount++;
		quaddata->IsDrawing = true;


	}
	//_________________________________________________________________________________________________________
}




