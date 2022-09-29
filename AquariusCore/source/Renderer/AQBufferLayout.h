#pragma once
#include <vector>
#include <GLAD/glad.h>
#include "core/Log.h"

namespace Aquarius
{
	


	enum class AQUARIUS_API BufferDataType
	{
		None = 0,
		Float1, Float2, Float3, Float4,
		Mat3F, Mat4F,
		Int1, Int2, Int3, Int4,
		Bool
	};

	struct AQUARIUS_API BufferElement
	{
		std::string Name;
		uint32_t BufferLocation;
		BufferDataType Type ;
		bool Normalized;
		uint32_t Offset ;
		uint32_t Count;


		BufferElement(){};
			

		BufferElement(const std::string& name, uint32_t  bufferlocation ,BufferDataType type,bool normalized=false)
			:Name(name), BufferLocation(bufferlocation),Type(type), Normalized(normalized), Offset(0), Count(0)
		{

		}

		constexpr int GetDimension() const
		{
			switch (Type)
			{
			case Aquarius::BufferDataType::Float1:	return 1;
			case Aquarius::BufferDataType::Float2:	return 2;
			case Aquarius::BufferDataType::Float3:	return 3;
			case Aquarius::BufferDataType::Float4:	return 4;
			case Aquarius::BufferDataType::Mat3F:	return  3 * 3;
			case Aquarius::BufferDataType::Mat4F:	return 4 * 4;
			case Aquarius::BufferDataType::Int1:		return 1;
			case Aquarius::BufferDataType::Int2:		return  2;
			case Aquarius::BufferDataType::Int3:		return  3;
			case Aquarius::BufferDataType::Int4:		return  4;
			case Aquarius::BufferDataType::Bool:		return 1;
			default:break;
			}
			AQ_ASSERT(false, "Unknown BufferDataType!")
				return 0;
		}

		constexpr uint32_t GetSingleSize() const
		{
			switch (Type)
			{
			case Aquarius::BufferDataType::Float1:	return sizeof(float);
			case Aquarius::BufferDataType::Float2:	return sizeof(float);
			case Aquarius::BufferDataType::Float3:	return sizeof(float);
			case Aquarius::BufferDataType::Float4:	return sizeof(float);
			case Aquarius::BufferDataType::Mat3F:	return  sizeof(float);
			case Aquarius::BufferDataType::Mat4F:	return sizeof(float);
			case Aquarius::BufferDataType::Int1:		return sizeof(int);
			case Aquarius::BufferDataType::Int2:		return  sizeof(int);
			case Aquarius::BufferDataType::Int3:		return  sizeof(int);
			case Aquarius::BufferDataType::Int4:		return  sizeof(int);
			case Aquarius::BufferDataType::Bool:		return sizeof(bool);
			default:break;
			}
			AQ_ASSERT(false, "Unknown BufferDataType!")
				return 0;
		}

		 
		constexpr GLenum  GetGLType() const
		{
			switch (Type)
			{
			case Aquarius::BufferDataType::Float1:	return GL_FLOAT;
			case Aquarius::BufferDataType::Float2:	return GL_FLOAT;
			case Aquarius::BufferDataType::Float3:	return GL_FLOAT;
			case Aquarius::BufferDataType::Float4:	return GL_FLOAT;
			case Aquarius::BufferDataType::Mat3F:	return GL_FLOAT;
			case Aquarius::BufferDataType::Mat4F:	return GL_FLOAT;
			case Aquarius::BufferDataType::Int1:		return GL_INT;
			case Aquarius::BufferDataType::Int2:		return GL_INT;
			case Aquarius::BufferDataType::Int3:		return GL_INT;
			case Aquarius::BufferDataType::Int4:		return GL_INT;
			case Aquarius::BufferDataType::Bool:		return GL_BYTE;
			default:break;
			}
			AQ_ASSERT(false, "Unknown BufferDataType!")
				return 0;
		}

	};


	class AQUARIUS_API AQBufferLayout
	{
	public:
		AQBufferLayout(const std::initializer_list<BufferElement>& elements);

		AQBufferLayout();
			

	public:
		inline const uint32_t GetStride()const { return m_Stride; };
		inline const std::vector<BufferElement>& GetElements()const { return m_Elements; };

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		void CalculateCount(int datasize);
	private:
		void CalculateOffsetAndStride();
		

	private:
		uint32_t m_Stride;
		std::vector<BufferElement> m_Elements;
		
	};

	
}


