#include "AQPCH.h"
#include "AQGUI.h"

namespace Aquarius
{



	void AQGUI::ReferenceDecrease(bool deallocated) const noexcept
	{
        --m_referenceCount;
        if (m_referenceCount == 0 && deallocated) { delete this;}
        else if (m_referenceCount < 0) 
        {
            AQ_CORE_ERROR("Internal error: AQGUI reference count < 0!");
            abort();
        }

	}


}