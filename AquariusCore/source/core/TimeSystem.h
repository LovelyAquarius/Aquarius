#pragma once
#include "AquariusCore.h"
#include <chrono>
#include <vector>

namespace Aquarius
{
	 class Application;


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
namespace Aquarius
{
	//Aquarius基准计时类
	template<typename Fn>
	class AQUARIUS_API AQTimer
	{
	public:

		AQTimer(const char* name, Fn&& function) :m_Name(name), m_Function(function),Stopped(false)
		{
			m_StartPoint = std::chrono::high_resolution_clock::now();
		}

		~AQTimer()
		{
			if (!Stopped)
				Stop();
		}
		void Stop()
		{
			auto endpoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endpoint).time_since_epoch().count();
			Stopped = true;

			float duration = (end - start) * 0.001f;
			m_Function({ m_Name, duration });
			
		}




	private:
		const char* m_Name;
		Fn m_Function;
		std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;
		bool Stopped;

	};

	struct ProfileMessage
	{
		const char* Name;
		float Time;
	};


#define AQ_TIME_MONITOR(name) Aquarius::AQTimer Timer_##__LINE__(name,[&](Aquarius::ProfileMessage profile){Aquarius::Application::s_profile.push_back(profile);})








}