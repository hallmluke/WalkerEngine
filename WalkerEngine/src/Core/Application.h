#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"

namespace Walker {

	class WALKER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void Run();

		static Application& Get() { return *s_Instance; }

		Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		static Application* s_Instance;
		float m_LastFrameTime = 0;
	};

	Application* CreateApplication();
}