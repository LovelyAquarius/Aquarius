#pragma once

#include "core/Input.h"

namespace Aquarius
{

	class AQUARIUS_API  WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int buttton) override;
		virtual std::pair<float, float>GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;


	};



}




