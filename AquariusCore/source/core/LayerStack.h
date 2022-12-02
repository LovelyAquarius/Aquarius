#pragma once
#include "AquariusCore.h"
#include "core/Layer.h"

namespace Aquarius
{

	class AQUARIUS_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
	
		
		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverLay(Layer* overlay);

		void DestroyLayers();//From head to back

		std::vector<Layer*>::iterator begin() { return m_Layers.begin();}
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*>m_Layers;
		unsigned int  m_LayerInsertIndex = 0;
	
	};














}