#pragma once
#include "AQPCH.h"
#include "AQObject.h"


namespace Aquarius
{

	void AQObject::ReferenceDecrease(bool deallocated) const noexcept
	{
        --m_referenceCount;
        if (m_referenceCount == 0 && deallocated) {
            delete this;
        }
        else if (m_referenceCount < 0) {
            AQ_CORE_ERROR("Internal error: Object reference count < 0!");
            abort();
        }
	}



}


