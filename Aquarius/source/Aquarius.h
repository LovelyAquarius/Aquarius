#pragma once
#include<string.h>
//核心组件
#include "core/Application.h"
#include "core/Log.h"
//_____________________-


//渲染组件
#include "Renderer/OrthgraphicCamera.h"
#include "Renderer/AQBufferLayout.h"
#include "Platform/AQOpenGL/AQGLShader.h"
#include "Renderer/AQBuffer.h"
#include "Renderer/Renderer.h"
#include "Platform/AQOpenGL/AQGLTexture.h"
#include "Data/3DObiects/MathModel/ThreeDModel.h"
#include <GLFW/glfw3.h>
//_____________________________

#include "IMGUI/imgui.h"
#include "backends/imgui_impl_opengl3.h"

//程序入口
#include "core/entrypoint.h"
//_____________________________________


//外部声明
//Aquarius::GraphicAPI Aquarius::RenderAPI::s_GraphicAPI= Aquarius::GraphicAPI::OpenGL;
//___________________________