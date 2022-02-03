#include "walkerpch.h"
#include "Application.h"
#include "ImGuiManager.h"

#include "GLFW/glfw3.h"

Walker::Application::Application()
{
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	s_Instance = this;
}

Walker::Application::~Application()
{
}

void Walker::Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	//W_CORE_TRACE("{0}", e.ToString());
}

void Walker::Application::Run()
{
	ImGuiManager imguiManager((GLFWwindow*) m_Window->GetNativeWindow());

	while (m_Running) {
		
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		imguiManager.BeginFrame();
		imguiManager.EndFrame();
		
		m_Window->OnUpdate();
		
	}
}

bool Walker::Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}
