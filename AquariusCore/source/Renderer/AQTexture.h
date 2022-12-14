#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include "MathSystem/AQMathEx.h"

namespace Aquarius
{
	class AQUARIUS_API AQTexture:public AQObject
	{
	public:
		static AQObjectType ClassType() { return AQObjectType::AQTexture; }
	public:
		virtual void LoadFile(const char* filepath) = 0;
		virtual void LoadData(void* data, AQUINT datasize) = 0;
		virtual const int GetWidth()const = 0;
		virtual const int GetHeight()const = 0;

	protected:
		AQTexture() { m_type = AQObjectType::AQTexture; }
	};

	class AQUARIUS_API AQTexture2D:public AQTexture 
	{
	public:
		static AQRef<AQTexture2D>  Create(const std::string name, const char* filepath);
		static AQRef<AQTexture2D>  Create(const char* filepath);
		static AQRef<AQTexture2D>  Create(const std::string name, AQUINT width, AQUINT height);
		static AQObjectType ClassType() { return AQObjectType::AQTexture2D; }
	public:
		virtual bool operator==( AQRef<AQTexture2D>& other)const = 0;


		virtual void Bind()const =0;
		virtual void UnBind()const =0;
		virtual void Delete()const  =0;
		virtual void BindSlot(unsigned int textureslot)=0;

		virtual void LoadFile(const char* filepath)=0;
		virtual void LoadData(void* data, uint32_t datasize)=0;

		virtual const int GetWidth()const = 0;
		virtual const int GetHeight()const = 0;
		virtual const unsigned int  GetTextureID()const = 0;

	protected:
		AQTexture2D() { m_type = AQObjectType::AQTexture2D; }
	};

	class AQUARIUS_API AQSubTexture2D :public AQObject
	{
	public:
		static AQRef<AQSubTexture2D> Create(const std::string name, const AQRef<AQTexture2D>& texture, const Eigen::Vector2f& subsize, const Eigen::Vector2f& subcoordindex);
		static AQRef<AQSubTexture2D> Create(const AQRef<AQTexture2D>& texture, const Eigen::Vector2f& subsize, const Eigen::Vector2f& subcoordindex);

		virtual const AQRef<AQTexture2D> GetTexture()const = 0;
		virtual const Eigen::Vector2f* GetTexCoords() const = 0;

	protected:
		AQSubTexture2D() { m_type = AQObjectType::AQSubTexture2D; }
	};
}


