#pragma once
#include <iostream>
#include "AquariusCore.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>


#ifdef AQ_DEBUG
	#define AQ_CORE_ERROR(...)  ::Aquarius::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define AQ_CORE_WARN(...)   ::Aquarius::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define AQ_CORE_INFO(...)   ::Aquarius::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define AQ_CORE_TRACE(...)  ::Aquarius::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define AQ_CORE_FATAL(...)  ::Aquarius::Log::GetCoreLogger()->fatal(__VA_ARGS__)

	#define AQ_CLIENT_ERROR(...)  ::Aquarius::Log::GetClientLogger()->error(__VA_ARGS__)
	#define AQ_CLIENT_WARN(...)   ::Aquarius::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define AQ_CLIENT_INFO(...)   ::Aquarius::Log::GetClientLogger()->info(__VA_ARGS__)
	#define AQ_CLIENT_TRACE(...)  ::Aquarius::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define AQ_CLIENT_FATAL(...)  ::Aquarius::Log::GetClientLogger()->fatal(__VA_ARGS__)

#endif // AQ_DEBUG



namespace Aquarius
{

	class AQUARIUS_API Log
{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger ;

};


}


