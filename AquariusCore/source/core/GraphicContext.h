#pragma once
#include "AquariusCore.h"
namespace Aquarius
{

	class AQUARIUS_API GraphicContext
	{
	public:

		GraphicContext() {};

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;


	};




}