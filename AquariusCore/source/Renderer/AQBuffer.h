#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include "AQBufferLayout.h"
#include "Platform/AQOpenGL/GLError.h"
namespace Aquarius
{
	class AQVertexArray;
	class AQBufferLayout;
	class Renderer;
	enum class GraphicAPI;

	class AQUARIUS_API AQVertexBuffer:public AQObject
	{
	public:
		static AQRef<AQVertexBuffer> Create(uint32_t datasize, const std::string name = "");
		static AQRef<AQVertexBuffer> Create(int datasize, const void* data, int datahandledtype, const std::string name = "");
		static AQRef<AQVertexBuffer> Create(const AQBufferLayout& layout,int datasize, const void* data, int datahandledtype,const std::string name = "");
		static AQObjectType ClassType() { return AQObjectType::AQVertexBuffer; }
	public:
		virtual void Bind() const=0;
		virtual void UnBind() const=0;
		virtual void Delete()const=0;
		virtual void SetData(const void* data, uint32_t datasize ) = 0;


		virtual  const AQBufferLayout& GetLayout()const=0;
		virtual   AQBufferLayout& GetLayout()=0;
		virtual void SetLayout(const AQBufferLayout& layout)=0;

	protected:
		AQVertexBuffer() { m_type = AQObjectType::AQVertexBuffer; }
	protected:
		AQBufferLayout m_Layout;
	};


	class AQUARIUS_API AQElementBuffer :public AQObject
	{
	public:
		static  AQRef<AQElementBuffer>  Create(uint32_t elementcount, const void* data, int datahandledtype, const std::string name = "");
		static AQObjectType ClassType() { return AQObjectType::AQElementBuffer; }
	public:

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Delete()const = 0;

	protected:
		AQElementBuffer() { m_type = AQObjectType::AQElementBuffer; };
	};

	class AQUARIUS_API AQVertexArray :public AQObject
	{
	public:
		static AQRef<AQVertexArray> Create(const std::string name="");
		static AQObjectType ClassType() { return AQObjectType::AQVertexArray; }
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Delete()const = 0;

		virtual void LinkVE(AQRef<AQVertexBuffer> vbo, AQRef<AQElementBuffer> ebo) = 0;

	protected:
		AQVertexArray() { m_type = AQObjectType::AQVertexArray; }
	};


}