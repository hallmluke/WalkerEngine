#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "Core/LayerStack.h"

namespace Walker {

	class WALKER_API Application
	{
	public:
		Application(const std::string& name);
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Run();

		static Application& Get() { return *s_Instance; }

		Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		static Application* s_Instance;
		float m_LastFrameTime = 0;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}