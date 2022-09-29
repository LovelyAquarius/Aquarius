#include "AQPCH.h"
#include "Log.h"

namespace Aquarius

{
	std::shared_ptr<spdlog::logger>  Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger>  Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Core");
		s_CoreLogger->set_level(spdlog::level::trace);


		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);

	
	 }





}
