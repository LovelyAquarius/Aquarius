#include "AQPCH.h"
#include "AQBufferLayout.h"

namespace Aquarius
{

	AQBufferLayout::AQBufferLayout(const std::initializer_list<BufferElement>& elements)
		:m_Elements(elements)
	{
		CalculateOffsetAndStride();
	}

	AQBufferLayout::AQBufferLayout()
		:m_Elements(std::vector<BufferElement>()) 
	{
		CalculateOffsetAndStride();
	}





	void AQBufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.GetDimension() * element.GetSingleSize();
			m_Stride += element.GetDimension() * element.GetSingleSize();
		}

	}

	void AQBufferLayout::CalculateCount(int datasize)
	{
		int lastrest = datasize % m_Stride;
		if (!lastrest)
		{
			int count = datasize / m_Stride;
			for (auto& element : m_Elements)
			{
				element.Count = count;
			}
		}
		else
		{
			int count= datasize / m_Stride;

			for (auto& element : m_Elements)
			{
				auto elementsize = element.GetDimension() * element.GetSingleSize();
				if (lastrest >= elementsize)
				{
					element.Count = count+1;
					lastrest -= elementsize;
				}
				else
				{
					element.Count = count;
					lastrest = 0;
				}
			}
		}

	}


}