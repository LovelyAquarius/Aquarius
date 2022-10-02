#pragma once
#include"AQPCH.h"
#include "Application.h"
#include "APPGUI.h"
#include "Platform/AQOpenGL/GLError.h"
#include "Renderer/Renderer.h"
#include "Utils/AQMonitor/AQMonitor.h"
#include <GLFW/glfw3.h>
namespace Aquarius
{
	Application* Application::s_Instance = nullptr;
	GraphicAPI RenderAPI::s_GraphicAPI = GraphicAPI::OpenGL;

	Application::Application()
		
	{
		AQ_CORE_ASSERT(!s_Instance, "Application alreaady exists!");
		s_Instance = this;
		//所有事件在这里触发
		m_Window->SetEventCallBack(AQ_BIND_EVENT_FN(Application::OnEvent));
		//_______________________________________
		//初始化渲染器
		Renderer::Init();
		//_______________________________________
		//创建appgui实例
		m_GUI = APPGUI::GetAPPGUI();
		PushOverLay(m_GUI);
		//_______________________________________
		// 初始化监控器
		AQApplicationMonitor::Init();
		PushOverLay(AQApplicationMonitor::s_Monitor);
		//_______________________________________
		//初始化配置
		m_Window->SetSync(true);         //垂直同步
		//_______________________________________
	}

	Application::~Application()
	{
	
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLay(Layer* layer)
	{
		m_LayerStack.PushOverLay(layer);
		layer->OnAttach();
	}
	void Application::OnEvent(BaseEvent& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(AQ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(AQ_BIND_EVENT_FN(Application::OnWindowResize));



		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); it)
		{
			(*--it)->OnEvent(event);

			if (event.m_Handled)
				break;
		}
	}

	void Application::OnUpdate(DeltaTime& dt)
	{
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate(dt);
	}

	void Application::OnGUIRender()
	{

		m_GUI->Begin();

		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();

		m_GUI->End();

	}
	void Application::OnRender()
	{
		for (Layer* layer : m_LayerStack)
			layer->OnRender();
	}

	void Application::Run()
	{
		while (Running)
		{
			//前置工作
			float time = glfwGetTime();
			DeltaTime dt = time - m_LastFrameTime;
			m_LastFrameTime = time;
			//____________________
			//APP先刷新
			OnUpdate(dt);
			//____________________
			//Layer渲染
			OnRender();
			//____________________
			//UI渲染
			OnGUIRender();
			//____________________
			//窗口环境最后刷新
			m_Window->OnUpdade();
			//________________________
		}
	}



	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		return false;
	}

}