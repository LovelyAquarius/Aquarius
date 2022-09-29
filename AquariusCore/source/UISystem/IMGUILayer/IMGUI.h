#pragma once
#include "core/AquariusCore.h"
#include "core/Layer.h"
#include "core/Input.h"
#include "core/KeyCode.h"
namespace Aquarius
{

	class AQUARIUS_API IMGUILayer :public Layer
	{
	public:
		IMGUILayer(const std::string name) :Layer(std::string("IMGUI->") + name) {};
	private:
	};









}