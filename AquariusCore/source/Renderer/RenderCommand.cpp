#include "AQPCH.h"
#include "RenderCommand.h"
#include "Platform/AQOpenGL/OpenGLRenderAPI.h"

namespace Aquarius
{
	RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI();

	

}