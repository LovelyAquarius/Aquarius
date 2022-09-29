#pragma once 
#include "core/AquariusCore.h"
#include "core/Layer.h"
#include "core/Input.h"
#include "core/KeyCode.h"

namespace Aquarius
{

	//所有GUI元素的基类
	class AQUARIUS_API AQGUI:public Layer
	{
	public:
		AQGUI(const std::string name) :Layer(std::string("AQGUI->")+name) {};
		AQGUI(const AQGUI& other) : m_referenceCount(0) {m_LayerName = other.m_LayerName;}

		int GetReferenceCount() const { return m_referenceCount; };
		void ReferenceIncrease() const { ++m_referenceCount; }
		void ReferenceDecrease(bool deallocated = true) const noexcept;


	private:
		mutable std::atomic<AQINT> m_referenceCount{ 0 };

	};

	






}






