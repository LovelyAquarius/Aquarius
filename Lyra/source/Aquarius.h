#pragma once
//___________________
#include<string.h>
//___________________
//核心组件
#include "core/Application.h"
#include "core/Log.h"
#include "core/AQCommon.h"
#include "core/RandomSystem.h"
#include "core/TimeSystem.h"
#include "EventSystem/ApplicationEvent.h"
#include "EventSystem/KeyEvent.h"
#include "EventSystem/MouseEvent.h"
//______________________
//渲染组件
#include "Renderer/OrthgraphicCameraController.h"
#include "Renderer/AQBufferLayout.h"
#include "Platform/AQOpenGL/AQGLShader.h"
#include "Renderer/AQBuffer.h"
#include "Renderer/AQFrameBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Platform/AQOpenGL/AQGLTexture.h"
#include "Data/3DObiects/MathModel/ThreeDModel.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
//_____________________________
// 场景组件
#include "Scene/AQScene.h"
#include "Scene/AQEntity.h"
// ————————————————————————————————————————
// 功能组件
#include "Renderer/ParticleSystem/AQParticle2D.h"
#include "Utils/AQGeometry/AQEuclidian2D.h"
// ————————————————————————————————————————————————————————



//外部声明
//Aquarius::GraphicAPI Aquarius::RenderAPI::s_GraphicAPI= Aquarius::GraphicAPI::OpenGL;
//___________________________