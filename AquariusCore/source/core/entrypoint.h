#pragma once

#ifdef AQ_PLATFORM_WINDOWS
#include"AquariusCore.h"
#include "Application.h"
#include "Log.h"
#include "Renderer/RenderAPI.h"


extern Aquarius::Application* Aquarius::CreateApplication();


int main(int argc, char** argv)
{
	Aquarius::Log::Init();
	AQ_CORE_INFO("当前操作系统：Windows！");
#ifdef AQ_DEBUG
	AQ_CORE_INFO("当前为调试模式！");
#endif
	AQ_CORE_INFO("核心日志系统已启动！");
	

	auto myapp = Aquarius::CreateApplication();
	myapp->Run();
	
}




#endif