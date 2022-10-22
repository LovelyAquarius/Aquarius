#pragma once
#include "AquariusCore.h"
#include "core/TimeSystem.h"
#include "EventSystem/EventSystem.h"

namespace Aquarius
{
	//AQ layer
	class  AQUARIUS_API Layer
	{
	public:
		Layer(const std::string& name = "UnnamedLayer");
		virtual ~Layer();

		virtual void  OnAttach() {}
		virtual void  OnDetach() {}
		virtual void  OnUpdate(DeltaTime& dt) {}
		virtual void  OnImGuiRender(){}
		virtual void	 OnRender(DeltaTime& dt){}

		virtual void  OnEvent(BaseEvent& event) {}

		inline const std::string GetLayerName() const { return m_LayerName; }

	protected:


		std::string m_LayerName;
	};


}

