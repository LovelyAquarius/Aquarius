#include "AQPCH.h"
#include "Renderer2D.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"
#include "Platform/AQOpenGL/AQGLTexture.h"

namespace Aquarius
{
	static Renderer2DData* s_Data2D;

	void Renderer2D::Init()
	{
		//创建默认数据
		s_Data2D = new Renderer2DData();
		Init_CommonTexture2D();
		Init_QuadVertex2D();
		//________________________________________________
		//创建自定义数据
		/*Init_LineVertex2D();
		Init_QudraticBezierUnit2D();*/
		//_______________________________________________________
	}

	void Renderer2D::ShutDown()
	{
		//删除默认的子数据，这些默认数据不可自定义删除
		s_Data2D->CommonTexture2D.reset(nullptr);
		s_Data2D->QuadVertex2D.reset(nullptr);
		//________________________________________________
		//删除自定义子数据
		for (auto& subdata : s_Data2D->OnUses)
		{
			ShutDown(subdata);
		}
		delete s_Data2D;
	}

	void Renderer2D::BeginScene(OrthgraphicCamera& camera)
	{
		for (auto& subdata : s_Data2D->OnUses)
		{
			//为着色器传入相机VP参数
			PassCameraVPIntoShader(camera.GetViewProjection(), subdata);
			//重置各个绘图子数据缓冲
			ResetSubData(subdata);
		}
	}

	void Renderer2D::BeginScene(AQCamera& camera, const Eigen::Matrix4f& transform)
	{
		Eigen::Matrix4f VP = camera.GetProjection() * transform.inverse();

		for (auto& subdata : s_Data2D->OnUses)
		{
			//为着色器传入相机VP参数
			PassCameraVPIntoShader(VP, subdata);
			//重置各个绘图子数据缓冲
			ResetSubData(subdata);
		}


	}

	void Renderer2D::Flush()
	{
		//绑定可用纹理槽
		for (AQUINT i = 0; i < s_Data2D->CommonTexture2D->TextureSlotIndex; i++)
			s_Data2D->CommonTexture2D->TextureSlots[i]->BindSlot(i);
		//_________________________________________________
		//DrawCall
		for (auto& subdata : s_Data2D->OnUses)
		{
			DrawSubData(subdata);
		}
		//___________________________________________________
	}

	void Renderer2D::ResetStatistics() { memset(&(s_Data2D->Statistics), 0, sizeof(Statistics)); }
	Renderer2D::Statistics Renderer2D::GetStatistics() { return s_Data2D->Statistics; }

	void Renderer2D::EndScene()
	{
		//传入各个VBO当前场景所需数据
		for (auto& subdata : s_Data2D->OnUses)
		{
			SubmitSubData(subdata);
		}
		//________________________________________________________-

		Flush();
	}

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
		AQ_CORE_EIGEN_IDENTITY_MATRIX4F(1.0f, transform.matrix());
		AQ_Scale_Self(transform.matrix(), Eigen::Vector3f(size.x(), size.y(), 1.0f));
		transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(rotation), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));
		transform.translate(position);



		SetQuadDataToVertexBuffer(transform.matrix(), color, textureindex, tilingfactor);

		s_Data2D->QuadVertex2D->IsDrawing = true;
	}

	void Renderer2D::DrawQuad(const Eigen::Matrix4f& transform, const Eigen::Vector4f& color)
	{
		const float textureindex = 0.0f;//这是纯白像素！
		constexpr float tilingfactor = 1.0f;
		SetQuadDataToVertexBuffer(transform, color, textureindex, tilingfactor);

		s_Data2D->QuadVertex2D->IsDrawing = true;
	}

	void Renderer2D::DrawQuad(const Eigen::Vector3f& position, const Eigen::Vector3f& aboutpoint, const AQFLOAT  rotation, const Eigen::Vector2f& size, const Eigen::Vector4f& color)
	{
		const float textureindex = 0.0f;//这是纯白像素！
		constexpr float tilingfactor = 10.0f;

		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		AQ_CORE_EIGEN_IDENTITY_MATRIX4F(1.0f, transform.matrix());
		AQ_Scale_Self(transform.matrix(), Eigen::Vector3f(size.x(), size.y(), 1.0f));
		transform.translate(-aboutpoint);
		transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(rotation), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));
		transform.translate(position + aboutpoint);

		SetQuadDataToVertexBuffer(transform.matrix(), color, textureindex, tilingfactor);

		s_Data2D->QuadVertex2D->IsDrawing = true;
	}


	void Renderer2D::DrawQuad(const Eigen::Vector2f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const AQRef<AQTexture2D>& texture, AQFLOAT tilingfactor)
	{
		DrawQuad(Eigen::Vector3f(position.x(), position.y(), 0.0f), rotation, size, texture, tilingfactor);
	}

	void Renderer2D::DrawQuad(const Eigen::Vector3f& position, const AQFLOAT  rotation, const Eigen::Vector2f& size, const AQRef<AQTexture2D>& texture, AQFLOAT tilingfactor)
	{
		const Eigen::Vector4f color{ 1.0f,1.0f,1.0f,1.0f };

		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		AQ_CORE_EIGEN_IDENTITY_MATRIX4F(1.0f, transform.matrix());
		AQ_Scale_Self(transform.matrix(), Eigen::Vector3f(size.x(), size.y(), 1.0f));
		transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(rotation), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));
		transform.translate(position);

		AQUINT textureindex = MatchTextureIndex(texture);
		if (!textureindex)
		{
			textureindex = s_Data2D->CommonTexture2D->TextureSlotIndex;
			s_Data2D->CommonTexture2D->TextureSlots[s_Data2D->CommonTexture2D->TextureSlotIndex] = texture;
			s_Data2D->CommonTexture2D->TextureSlotIndex++;
		}
		SetQuadDataToVertexBuffer(transform.matrix(), color, (float)textureindex, tilingfactor);

		s_Data2D->QuadVertex2D->IsDrawing = true;
	}

	void Renderer2D::DrawQuad(const Eigen::Vector3f& position, const Eigen::Vector3f& aboutpoint, const AQFLOAT  rotation, const Eigen::Vector2f& size, const AQRef<AQTexture2D>& texture, AQFLOAT tilingfactor)
	{
		const Eigen::Vector4f color{ 1.0f,1.0f,1.0f,1.0f };

		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		AQ_CORE_EIGEN_IDENTITY_MATRIX4F(1.0f, transform.matrix());
		AQ_Scale_Self(transform.matrix(), Eigen::Vector3f(size.x(), size.y(), 1.0f));
		transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(rotation), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));
		transform.translate(position);

		AQUINT textureindex = MatchTextureIndex(texture);
		if (!textureindex)
		{
			textureindex = s_Data2D->CommonTexture2D->TextureSlotIndex;
			s_Data2D->CommonTexture2D->TextureSlots[s_Data2D->CommonTexture2D->TextureSlotIndex] = texture;
			s_Data2D->CommonTexture2D->TextureSlotIndex++;
		}
		SetQuadDataToVertexBuffer(transform.matrix(), color, (float)textureindex, tilingfactor);

		s_Data2D->QuadVertex2D->IsDrawing = true;
	}
	//_________________________________________________________________________________________________________
	
	//DrawBezier___________________________________________________________________________________________
	void Renderer2D::DrawBezier_Line_CPU(const Eigen::Vector3f& startpoint, const Eigen::Vector3f& endpoint, const Eigen::Vector3f& controlpoint, AQUINT count, const Eigen::Vector4f& color)
	{
		if (!s_Data2D->LineVertex2D)
		{
			AQ_CORE_WARN("Renderer2D::DrawBezier_Line_CPU;Failed to submit data,the data of LineVertex2D is not inicialized!");
			return;
		}
		//检查Line缓冲区是否已满
		AQUINT linevertexcount = 0;
		for (auto& elementcount : s_Data2D->LineVertex2D->LineElementCounts)
		{ 
			linevertexcount += elementcount;
		}
		if (linevertexcount >= s_Data2D->LineVertex2D->MaxLineIndices-count)
			FlushAndReset();
		//________________________________________________

		float step = 1.0f / (float)count;
		float t = 0.0f;
		for (AQUINT index = 0; index < count; index ++)
		{
			s_Data2D->LineVertex2D->LineVertexPointer->Position = CalculateBezierPosition(startpoint, endpoint, controlpoint, t);
			s_Data2D->LineVertex2D->LineVertexPointer->Color = color;
			t += step;
			s_Data2D->LineVertex2D->LineVertexPointer++;
		}
		s_Data2D->LineVertex2D->LineElementCounts.emplace_back(count);
		s_Data2D->Statistics.LineVertexCount += count;
		s_Data2D->LineVertex2D->IsDrawing = true;
	}
	void Renderer2D::DrawBezier_Line_CPU(const AQRef<AQQuadraticBezierCurve2D>& beziercurve, AQUINT count, const Eigen::Vector4f& color)
	{
		if (!s_Data2D->LineVertex2D)
		{
			AQ_CORE_WARN("Renderer2D::DrawBezier_Line_CPU;Failed to submit data,the data of LineVertex2D is not inicialized!");
			return;
		}
		int curvecount = (int)beziercurve->GetControls().size();

		//检查Line缓冲区是否已满
		AQUINT linevertexcount = 0;
		for (auto& elementcount : s_Data2D->LineVertex2D->LineElementCounts)
		{
			linevertexcount += elementcount;
		}
		if (linevertexcount >= s_Data2D->LineVertex2D->MaxLineIndices - count* curvecount)
			FlushAndReset();
		//________________________________________________

		AQUINT totalcount = 0;

		for (int i = 0; i < curvecount; i++)
		{
			Eigen::Vector3f start{ beziercurve->GetPoints()[i].x,beziercurve->GetPoints()[i].y,0.0f };
			Eigen::Vector3f end{ beziercurve->GetPoints()[i + 1].x,beziercurve->GetPoints()[i + 1].y,0.0f };
			Eigen::Vector3f control{ beziercurve->GetControls()[i].x,beziercurve->GetControls()[i].y,0.0f };
			
			float step = 1.0f / (float)count;
			float t = 0.0f;
			for (AQUINT index = 0; index < count; index++)
			{
				s_Data2D->LineVertex2D->LineVertexPointer->Position = CalculateBezierPosition(start, end, control, t);
				s_Data2D->LineVertex2D->LineVertexPointer->Color = color;
				t += step;
				s_Data2D->LineVertex2D->LineVertexPointer++;
			}
			totalcount += count;
		}

		s_Data2D->LineVertex2D->LineElementCounts.emplace_back(totalcount);
		s_Data2D->Statistics.LineVertexCount += totalcount;
		s_Data2D->LineVertex2D->IsDrawing = true;
	}
	void Renderer2D::DrawBezier_Line_GPU(const Eigen::Vector3f& startpoint, const Eigen::Vector3f& endpoint, const Eigen::Vector3f& controlpoint, AQUINT count, const Eigen::Vector4f& color, const AQFLOAT width)
	{
		if (!s_Data2D->QudraticBezierUnit2D)
		{
			AQ_CORE_WARN("Renderer2D::DrawBezier_Line_GPU;Failed to submit data,the data of QudraticBezierUnit2D is not inicialized!");
			return;
		}
		//检查Line缓冲区是否已满
		AQUINT unitcount = 0;
		for (auto& elementcount : s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts)
		{
			unitcount += elementcount;
		}
		if (unitcount >= s_Data2D->QudraticBezierUnit2D->MaxQuadraticBezierUnitIndices-1)
			FlushAndReset();
		//________________________________________________
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->start = startpoint;
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->end = endpoint;
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->control = controlpoint;
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->vertexcount = (float)count;
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->Color = color;
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer++;

		if (!s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth.size())
		{
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth.emplace_back(width);
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts.emplace_back(1);
		}
		else if(s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth.back()==width)
		{
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts.back()++;
		}
		else
		{
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth.emplace_back(width);
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts.emplace_back(1);
		}
		s_Data2D->Statistics.QuadraticBezierUnitCount++;
		s_Data2D->QudraticBezierUnit2D->IsDrawing = true;
	}
	void Renderer2D::DrawBezier_Line_GPU(const AQRef<AQQuadraticBezierCurve2D>& beziercurve, AQUINT count, const Eigen::Vector4f& color, const AQFLOAT width)
	{
		if (!s_Data2D->QudraticBezierUnit2D)
		{
			AQ_CORE_WARN("Renderer2D::DrawBezier_Line_GPU;Failed to submit data,the data of QudraticBezierUnit2D is not inicialized!");
			return;
		}
		int curvecount =(int) beziercurve->GetControls().size();

		//检查Line缓冲区是否已满
		AQUINT unitcount = 0;
		for (auto& elementcount : s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts)
		{
			unitcount += elementcount;
		}
		if (unitcount >= s_Data2D->QudraticBezierUnit2D->MaxQuadraticBezierUnitIndices - curvecount)
			FlushAndReset();

		for (int i = 0; i < curvecount; i++)
		{
			Eigen::Vector3f start{ beziercurve->GetPoints()[i].x,beziercurve->GetPoints()[i].y,0.0f };
			Eigen::Vector3f end{ beziercurve->GetPoints()[i + 1].x,beziercurve->GetPoints()[i + 1].y,0.0f };
			Eigen::Vector3f control{ beziercurve->GetControls()[i].x,beziercurve->GetControls()[i].y,0.0f };

			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->start = start;
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->end = end;
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->control = control;
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->vertexcount = (float)count;
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer->Color = color;
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer++;

		}

		if (!s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth.size())
		{
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth.emplace_back(width);
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts.emplace_back(curvecount);
		}
		else if (s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth.back() == width)
		{
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts.back()+= curvecount;
		}
		else
		{
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth.emplace_back(width);
			s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts.emplace_back(curvecount);
		}

		s_Data2D->Statistics.QuadraticBezierUnitCount+= curvecount;
		s_Data2D->QudraticBezierUnit2D->IsDrawing = true;
	}
	//_____________________________________________________________________________________________________________
}


namespace Aquarius
{
	void Renderer2D::FlushAndReset()
	{
		EndScene();

		for (auto& subdata : s_Data2D->OnUses)
		{
			ResetSubData(subdata);
		}
	}
	AQUINT Renderer2D::MatchTextureIndex(const AQRef<AQTexture2D>& texture)
	{
		for (AQUINT i = 1; i < s_Data2D->CommonTexture2D->TextureSlotIndex; i++)
		{
			if (s_Data2D->CommonTexture2D->TextureSlots[i] == texture)return i;
		}
		return 0;
	}

	void Renderer2D::SetQuadDataToVertexBuffer(const Eigen::Matrix4f& transform, const Eigen::Vector4f& color, AQFLOAT textureindex, AQFLOAT tilingfactor)
	{
		//检查Quad缓冲区是否已满
		if (s_Data2D->QuadVertex2D->QuadElementCount >= s_Data2D->QuadVertex2D->MaxQuadIndices - 6)
			FlushAndReset();
		//______________________________________________________
		//矩形左下点
		auto pos1 =transform* s_Data2D->QuadVertex2D->QuadPosition[0];
		s_Data2D->QuadVertex2D->QuadVertexPointer->Position << pos1.x(), pos1.y(), pos1.z(), pos1.w();
		s_Data2D->QuadVertex2D->QuadVertexPointer->Color <<color.x(), color.y(), color.z(), color.w();
		s_Data2D->QuadVertex2D->QuadVertexPointer->TexCoord << 0.0f, 0.0f;
		s_Data2D->QuadVertex2D->QuadVertexPointer->TexIndex = textureindex;
		s_Data2D->QuadVertex2D->QuadVertexPointer->TilingFactor = tilingfactor;
		s_Data2D->QuadVertex2D->QuadVertexPointer++;
		//______________________________________________________
		//矩形右下点
		auto pos2 = transform * s_Data2D->QuadVertex2D->QuadPosition[1];
		s_Data2D->QuadVertex2D->QuadVertexPointer->Position << pos2.x(), pos2.y(), pos2.z(), pos2.w();
		s_Data2D->QuadVertex2D->QuadVertexPointer->Color << color.x(), color.y(), color.z(), color.w();
		s_Data2D->QuadVertex2D->QuadVertexPointer->TexCoord << 1.0f, 0.0f;
		s_Data2D->QuadVertex2D->QuadVertexPointer->TexIndex = textureindex;
		s_Data2D->QuadVertex2D->QuadVertexPointer->TilingFactor = tilingfactor;
		s_Data2D->QuadVertex2D->QuadVertexPointer++;
		//______________________________________________________
		//矩形右上点
		auto pos3 = transform * s_Data2D->QuadVertex2D->QuadPosition[2];
		s_Data2D->QuadVertex2D->QuadVertexPointer->Position << pos3.x(), pos3.y(), pos3.z(), pos3.w();
		s_Data2D->QuadVertex2D->QuadVertexPointer->Color << color.x(), color.y(), color.z(), color.w();
		s_Data2D->QuadVertex2D->QuadVertexPointer->TexCoord << 1.0f, 1.0f;
		s_Data2D->QuadVertex2D->QuadVertexPointer->TexIndex = textureindex;
		s_Data2D->QuadVertex2D->QuadVertexPointer->TilingFactor = tilingfactor;
		s_Data2D->QuadVertex2D->QuadVertexPointer++;
		//______________________________________________________
		//矩形左上点
		auto pos4 = transform * s_Data2D->QuadVertex2D->QuadPosition[3];
		s_Data2D->QuadVertex2D->QuadVertexPointer->Position << pos4.x(), pos4.y(), pos4.z(), pos4.w();
		s_Data2D->QuadVertex2D->QuadVertexPointer->Color << color.x(), color.y(), color.z(), color.w();
		s_Data2D->QuadVertex2D->QuadVertexPointer->TexCoord << 0.0f, 1.0f;
		s_Data2D->QuadVertex2D->QuadVertexPointer->TexIndex = textureindex;
		s_Data2D->QuadVertex2D->QuadVertexPointer->TilingFactor = tilingfactor;
		s_Data2D->QuadVertex2D->QuadVertexPointer++;
		//______________________________________________________
		((s_Data2D->QuadVertex2D->QuadVertexBase) + 0)->Color;
		((s_Data2D->QuadVertex2D->QuadVertexBase) + 1)->Color;
		((s_Data2D->QuadVertex2D->QuadVertexBase) + 2)->Color;
		((s_Data2D->QuadVertex2D->QuadVertexBase) + 3)->Color;
		s_Data2D->QuadVertex2D->QuadElementCount += 6;
		s_Data2D->Statistics.QuadCount++;
	}
}

namespace Aquarius	 
{
	void Renderer2D::Init_CommonTexture2D()
	{
		if (!s_Data2D->CommonTexture2D)
			s_Data2D->CommonTexture2D = std::make_unique<CommonTexture2DData>();
		else
			AQ_CORE_WARN("Renderer2D::Init_CommonTexture2D: CommonTexture2DData is a default data, its not allowed to reset the data!");

		//配置纹理槽
		int32_t samplers[CommonTexture2DData::MaxTextureSlots];//对每个纹理槽采样
		for (int32_t i = 0; i < CommonTexture2DData::MaxTextureSlots; i++)
			samplers[i] = i;
		//______________________________________________________________
		//设置纯白像素块,把它作成一个单独的纹理是一个高效的方案
		AQUINT WhiteTextureData = 0xffffffff;
		s_Data2D->CommonTexture2D->DefaultTexture2D = AQTexture2D::Create("SingleWhitePixel", 1, 1);
		s_Data2D->CommonTexture2D->DefaultTexture2D->LoadData(&WhiteTextureData, sizeof(AQUINT));

		const char* whitetextureshader = R"(.\source\Assets\Internal\Shader\texture2d.glsl)";
		s_Data2D->CommonTexture2D->DefaultTextureShader2D = AQShader::Create(whitetextureshader);
		s_Data2D->CommonTexture2D->DefaultTextureShader2D->Bind();
		s_Data2D->CommonTexture2D->DefaultTextureShader2D->SetValue("u_Texture", samplers, CommonTexture2DData::MaxTextureSlots);

		s_Data2D->CommonTexture2D->TextureSlots[0] = s_Data2D->CommonTexture2D->DefaultTexture2D;
		//________________________________________________________________
		if(std::find(s_Data2D->OnUses.begin(), s_Data2D->OnUses.end(), Renderer2D_On_Use::CommonTexture2D)== s_Data2D->OnUses.end())
			s_Data2D->OnUses.emplace_back(Renderer2D_On_Use::CommonTexture2D);
	}

	void Renderer2D::Init_QuadVertex2D(AQUINT maxquads)
	{
		if (!s_Data2D->QuadVertex2D)
			s_Data2D->QuadVertex2D = std::make_unique<QuadVertex2DData>(maxquads);
		else
		{
			if (s_Data2D->QuadVertex2D->IsDrawing)
				AQ_CORE_WARN("Renderer2D::Init_QuadVertex2D: The data of QuadVertex2D has been using now, its not allowed to reset the data!");
			else
				s_Data2D->QuadVertex2D.reset(new QuadVertex2DData(maxquads));
		}

		s_Data2D->QuadVertex2D->QuadPosition[0] = Eigen::Vector4f{ -0.5f,-0.5f,0.0f,1.0f };
		s_Data2D->QuadVertex2D->QuadPosition[1] = Eigen::Vector4f{ 0.5f,-0.5f,0.0f,1.0f };
		s_Data2D->QuadVertex2D->QuadPosition[2] = Eigen::Vector4f{ 0.5f,0.5f,0.0f,1.0f };
		s_Data2D->QuadVertex2D->QuadPosition[3] = Eigen::Vector4f{ -0.5f,0.5f,0.0f,1.0f };

		s_Data2D->QuadVertex2D->QuadVAO2D = AQVertexArray::Create("quad2d");
		s_Data2D->QuadVertex2D->QuadVertexBase = new QuadVertex2DBuffer[s_Data2D->QuadVertex2D->MaxQuadVertices];
		s_Data2D->QuadVertex2D->QuadVBO2D = AQVertexBuffer::Create(s_Data2D->QuadVertex2D->MaxQuadVertices * sizeof(QuadVertex2DBuffer), "s_Data2D->QuadVBO2D");
		s_Data2D->QuadVertex2D->QuadVBO2D->SetLayout({
			{ "vertex",0,BufferDataType::Float4 },
			{ "color",1,BufferDataType::Float4 },
			{ "texcoord",2,BufferDataType::Float2 },
			{ "texindex",3,BufferDataType::Float1 } ,
			{ "tilingfactor",4,BufferDataType::Float1 } });

		AQUINT* quad2dindices = new AQUINT[s_Data2D->QuadVertex2D->MaxQuadIndices];
		AQUINT offset = 0;
		for (AQUINT i = 0; i < s_Data2D->QuadVertex2D->MaxQuadIndices; i += 6)
		{
			quad2dindices[i + 0] = offset + 0;
			quad2dindices[i + 1] = offset + 1;
			quad2dindices[i + 2] = offset + 2;
			quad2dindices[i + 3] = offset + 2;
			quad2dindices[i + 4] = offset + 3;
			quad2dindices[i + 5] = offset + 0;
			offset += 4;
		}

		s_Data2D->QuadVertex2D->QuadEBO2D = AQElementBuffer::Create(s_Data2D->QuadVertex2D->MaxQuadIndices, quad2dindices, GL_DYNAMIC_DRAW, "QuadEBO2D");
		delete[] quad2dindices;
		AQRefCast<AQGLVertexArray>(s_Data2D->QuadVertex2D->QuadVAO2D)->LinkVE(s_Data2D->QuadVertex2D->QuadVBO2D, s_Data2D->QuadVertex2D->QuadEBO2D);
		AQRefCast<AQGLVertexArray>(s_Data2D->QuadVertex2D->QuadVAO2D)->Enable(0, 1, 2, 3, 4);


		if (std::find(s_Data2D->OnUses.begin(), s_Data2D->OnUses.end(), Renderer2D_On_Use::QuadVertex2D) == s_Data2D->OnUses.end())
			s_Data2D->OnUses.emplace_back(Renderer2D_On_Use::QuadVertex2D);
	 }

	void Renderer2D::Init_LineVertex2D(AQUINT maxlinevertices)
	{
		if (!s_Data2D->LineVertex2D)
			s_Data2D->LineVertex2D = std::make_unique<LineVertex2DData>(maxlinevertices);
		else
		{
			if (s_Data2D->LineVertex2D->IsDrawing)
				AQ_CORE_WARN("Renderer2D::Init_LineVertex2D: The data of LineVertex2D has been using now, its not allowed to reset the data!");
			else
				s_Data2D->LineVertex2D.reset(new LineVertex2DData(maxlinevertices));
		}

		s_Data2D->LineVertex2D->LineVAO2D = AQVertexArray::Create("LineVAO2D");
		s_Data2D->LineVertex2D->LineVertexBase = new LineVertex2DBuffer[s_Data2D->LineVertex2D->MaxLineVertices];
		s_Data2D->LineVertex2D->LineVBO2D = AQVertexBuffer::Create(s_Data2D->LineVertex2D->MaxLineVertices * sizeof(LineVertex2DBuffer), "LineVBO2D");
		s_Data2D->LineVertex2D->LineVBO2D->SetLayout({
			{ "vertex", 0, BufferDataType::Float3 } ,
			{ "color",   1,  BufferDataType::Float4 } });

		AQUINT* lineindices = new AQUINT[s_Data2D->LineVertex2D->MaxLineIndices];
		for (AQUINT i = 0; i < s_Data2D->LineVertex2D->MaxLineIndices; i++)
		{
			lineindices[i] = i;
		}

		s_Data2D->LineVertex2D->LineEBO2D = Aquarius::AQElementBuffer::Create(s_Data2D->LineVertex2D->MaxLineIndices, lineindices, GL_DYNAMIC_DRAW, "LineEBO2D");
		delete[] lineindices;
		s_Data2D->LineVertex2D->LineVAO2D->LinkVE(s_Data2D->LineVertex2D->LineVBO2D, s_Data2D->LineVertex2D->LineEBO2D);
		AQRefCast<AQGLVertexArray>(s_Data2D->LineVertex2D->LineVAO2D)->Enable(0, 1);

		//安装线条着色器,着色器可以更换
		const char* lineshader = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\renderer2d_line.glsl)";
		s_Data2D->LineVertex2D->LineShader2D = AQShader::Create(lineshader);
		//_______________________________________________________________
		if (std::find(s_Data2D->OnUses.begin(), s_Data2D->OnUses.end(), Renderer2D_On_Use::LineVertex2D) == s_Data2D->OnUses.end())
			s_Data2D->OnUses.emplace_back(Renderer2D_On_Use::LineVertex2D);

	}

	void Renderer2D::Init_QudraticBezierUnit2D(AQUINT maxquadraticbezierunits)
	{
		if (!s_Data2D->QudraticBezierUnit2D)
			s_Data2D->QudraticBezierUnit2D = std::make_unique<QudraticBezierUnit2DData>(maxquadraticbezierunits);
		else
		{
			if (s_Data2D->QudraticBezierUnit2D->IsDrawing)
				AQ_CORE_WARN("Renderer2D::Init_QudraticBezierUnit2D: The data of QudraticBezierUnit2D has been using now, its not allowed to reset the data!");
			else
				s_Data2D->QudraticBezierUnit2D.reset(new QudraticBezierUnit2DData(maxquadraticbezierunits));
		}

		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitVAO2D = AQVertexArray::Create("QuadraticBezierUnitVAO2D");
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitBase = new QudraticBezierUnit2DBuffer[s_Data2D->QudraticBezierUnit2D->MaxQuadraticBezierUnits];
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitVBO2D = AQVertexBuffer::Create(s_Data2D->QudraticBezierUnit2D->MaxQuadraticBezierUnits * sizeof(QudraticBezierUnit2DBuffer), "QuadraticBezierUnitVBO2D");
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitVBO2D->SetLayout({
			{ "start", 0, BufferDataType::Float3 } ,
			{ "end", 1, BufferDataType::Float3 } ,
			{ "control", 2, BufferDataType::Float3 } ,
			{ "vertexcount", 3, BufferDataType::Float1 } ,
			{ "color",   4,  BufferDataType::Float4 } });

		AQUINT* quadraticbezierindices = new AQUINT[s_Data2D->QudraticBezierUnit2D->MaxQuadraticBezierUnitIndices];
		for (AQUINT i = 0; i < s_Data2D->QudraticBezierUnit2D->MaxQuadraticBezierUnitIndices; i++)
		{
			quadraticbezierindices[i] = i;
		}

		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitEBO2D = Aquarius::AQElementBuffer::Create(s_Data2D->QudraticBezierUnit2D->MaxQuadraticBezierUnitIndices, quadraticbezierindices, GL_DYNAMIC_DRAW, "QuadraticBezierUnitEBO2D");
		delete[] quadraticbezierindices;
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitVAO2D->LinkVE(s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitVBO2D, s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitEBO2D);
		AQRefCast<AQGLVertexArray>(s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitVAO2D)->Enable(0, 1, 2, 3, 4);

		//安装二阶Bezier着色器
		const char* quadraticbeziershader = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\renderer2d_quadraticbezier_unit.glsl)";
		s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitShader2D = AQShader::Create(quadraticbeziershader);
		//_______________________________________________________________

		if (std::find(s_Data2D->OnUses.begin(), s_Data2D->OnUses.end(), Renderer2D_On_Use::QudraticBezierUnit2D) == s_Data2D->OnUses.end())
			s_Data2D->OnUses.emplace_back(Renderer2D_On_Use::QudraticBezierUnit2D);

	}



	void Renderer2D::ResetSubData(Renderer2D_On_Use subdata)
	{
		switch (subdata)
		{
			case(Renderer2D_On_Use::CommonTexture2D):
			{
				s_Data2D->CommonTexture2D->TextureSlotIndex = 1;//第一个默认占用
				return;
			}
			case(Renderer2D_On_Use::QuadVertex2D):
			{
				s_Data2D->QuadVertex2D->QuadElementCount = 0;
				s_Data2D->QuadVertex2D->QuadVertexPointer = s_Data2D->QuadVertex2D->QuadVertexBase;
				return;
			}
			case(Renderer2D_On_Use::LineVertex2D):
			{
				std::vector<AQUINT>().swap(s_Data2D->LineVertex2D->LineElementCounts);
				s_Data2D->LineVertex2D->LineVertexPointer = s_Data2D->LineVertex2D->LineVertexBase;
				return;
			}
			case(Renderer2D_On_Use::QudraticBezierUnit2D):
			{
				std::vector<float>().swap(s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth);
				std::vector<AQUINT>().swap(s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts);
				s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer = s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitBase;
				return;
			}

		}
		return;
	}

	void Renderer2D::SubmitSubData(Renderer2D_On_Use subdata)
	{
		switch (subdata)
		{
			case(Renderer2D_On_Use::CommonTexture2D):return;
			case(Renderer2D_On_Use::QuadVertex2D):
			{
				AQUINT quadsize = (uint8_t*)s_Data2D->QuadVertex2D->QuadVertexPointer - (uint8_t*)s_Data2D->QuadVertex2D->QuadVertexBase;
				s_Data2D->QuadVertex2D->QuadVBO2D->SetData(s_Data2D->QuadVertex2D->QuadVertexBase, quadsize);
				return;
			}
			case(Renderer2D_On_Use::LineVertex2D):
			{
				AQUINT linesize =(uint8_t*)s_Data2D->LineVertex2D->LineVertexPointer - (uint8_t*)s_Data2D->LineVertex2D->LineVertexBase;
				s_Data2D->LineVertex2D->LineVBO2D->SetData(s_Data2D->LineVertex2D->LineVertexBase, linesize);
				return;
			}
			case(Renderer2D_On_Use::QudraticBezierUnit2D):
			{
				AQUINT quadraticbeziersize = (uint8_t*)s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitPointer - (uint8_t*)s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitBase;
				s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitVBO2D->SetData(s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitBase, quadraticbeziersize);
				return;
			}
		}
	}

	void Renderer2D::DrawSubData(Renderer2D_On_Use subdata)
	{
		switch (subdata)
		{
			case(Renderer2D_On_Use::CommonTexture2D):return;
			case(Renderer2D_On_Use::QuadVertex2D):
			{
				s_Data2D->CommonTexture2D->DefaultTextureShader2D->Bind();
				RenderCommand::DrawTriangleElement(s_Data2D->QuadVertex2D->QuadVAO2D, s_Data2D->QuadVertex2D->QuadElementCount);
				s_Data2D->Statistics.DrawCalls++;
				s_Data2D->QuadVertex2D->IsDrawing = false;
				return;
			}
			case(Renderer2D_On_Use::LineVertex2D):
			{
				s_Data2D->LineVertex2D->LineShader2D->Bind();
				RenderCommand::DrawLineElement(s_Data2D->LineVertex2D->LineVAO2D, s_Data2D->LineVertex2D->LineElementCounts);
				s_Data2D->Statistics.DrawCalls += (AQUINT)s_Data2D->LineVertex2D->LineElementCounts.size();
				s_Data2D->LineVertex2D->IsDrawing = false;
				return;
			}
			case(Renderer2D_On_Use::QudraticBezierUnit2D):
			{
				s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitShader2D->Bind();
				for (AQUINT i = 0; i < s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts.size(); i++)
				{
					RenderCommand::SetLineWidth(s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitLineWidth[i]);
					RenderCommand::DrawPointElement(s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitVAO2D, s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts[i]);
				}
				s_Data2D->Statistics.DrawCalls += (AQUINT)s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitElementCounts.size();
				s_Data2D->QudraticBezierUnit2D->IsDrawing = false;
				return;
			}
		}
		return;
	}
	void Renderer2D::PassCameraVPIntoShader(const Eigen::Matrix4f& camera_vp, Renderer2D_On_Use subdata)
	{
		switch (subdata)
		{
			case(Renderer2D_On_Use::CommonTexture2D):
			{
				s_Data2D->CommonTexture2D->DefaultTextureShader2D->Bind();
				s_Data2D->CommonTexture2D->DefaultTextureShader2D->SetValue("u_VP", camera_vp);
				return;
			}
			case(Renderer2D_On_Use::QuadVertex2D):
			{
				return;//暂时使用通用的纹理着色器
			}
			case(Renderer2D_On_Use::LineVertex2D):
			{
				s_Data2D->LineVertex2D->LineShader2D->Bind();
				s_Data2D->LineVertex2D->LineShader2D->SetValue("u_VP", camera_vp);
				return;
			}
			case(Renderer2D_On_Use::QudraticBezierUnit2D):
			{
				s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitShader2D->Bind();
				s_Data2D->QudraticBezierUnit2D->QuadraticBezierUnitShader2D->SetValue("u_VP", camera_vp);
				return;
			}

		}
		return;
	}

	void Renderer2D::ShutDown(Renderer2D_On_Use subdata)
	{
		if (!s_Data2D->OnUses.size())
			return;

		switch (subdata)
		{
		case(Renderer2D_On_Use::CommonTexture2D): return;//默认子数据不可删除
		case(Renderer2D_On_Use::QuadVertex2D): return;//默认子数据不可删除
		case(Renderer2D_On_Use::LineVertex2D):
			{
				auto it = std::find(s_Data2D->OnUses.begin(), s_Data2D->OnUses.end(), Renderer2D_On_Use::LineVertex2D);
				if (it != s_Data2D->OnUses.end())
					if (s_Data2D->LineVertex2D->IsDrawing)
					{
						AQ_CORE_WARN("Renderer2D::ShutDown: The data of LineVertex2D has been using now, its not allowed to delete the data!");
						return;
					}
					else
					{
						s_Data2D->LineVertex2D.reset(nullptr);
						s_Data2D->OnUses.erase(it);
						return;
					}
				else
					return;
			}
		case(Renderer2D_On_Use::QudraticBezierUnit2D):
			{
				auto it = std::find(s_Data2D->OnUses.begin(), s_Data2D->OnUses.end(), Renderer2D_On_Use::QudraticBezierUnit2D);
				if (it != s_Data2D->OnUses.end())
					if (s_Data2D->QudraticBezierUnit2D->IsDrawing)
					{
						AQ_CORE_WARN("Renderer2D::ShutDown: The data of QudraticBezierUnit2D has been using now, its not allowed to delete the data!");
						return;
					}
					else
					{
						s_Data2D->QudraticBezierUnit2D.reset(nullptr);
						s_Data2D->OnUses.erase(it);
						return;
					}
				else
					return;
			}
		}
		return;
	}

}

