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
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();
}