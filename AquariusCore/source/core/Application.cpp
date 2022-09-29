#pragma once
#include"AQPCH.h"

#include "APPGUI.h"
#include "Application.h"
#include "Platform/AQOpenGL/GLError.h"
#include "Renderer/RenderCommand.h"
#include"Platform/AQOpenGL/AQGLShader.h"
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
		

		//创建appgui实例
		m_GUI = APPGUI::GetAPPGUI();
		PushOverLay(m_GUI);
		//_______________________-
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

	void Application::OnUpdate()
	{
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();
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
		std::cout << (int)RenderAPI::GetGraphicAPI() << std::endl;
		while (Running)
		{
			//APP先刷新
			OnUpdate();
			//____________________
			//UI先渲染
			OnGUIRender();
			//____________________
			//Layer渲染
			OnRender();
			//____________________
			//窗口环境最后刷新
			m_Window->OnUpdade();
			//_____________________---
		}
	}



	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		OnUpdate();
		return false;
	}

}