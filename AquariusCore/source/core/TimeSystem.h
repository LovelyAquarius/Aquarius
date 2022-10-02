#pragma once
#include "AquariusCore.h"

namespace Aquarius

{

	class AQUARIUS_API DeltaTime
	{
	public:
		DeltaTime(float time =0.0f):m_Time(time){}

		operator float() { return m_Time;}
	public:
		float GetSeconds()const { return m_Time; }
		float GetMilliseconds()const { return m_Time * 1000.0f; }
	private:
		float m_Time;

	};







}