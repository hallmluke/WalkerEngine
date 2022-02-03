#include "walkerpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Walker {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		//W_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		//HZ_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		//W_CORE_ASSERT(status, "Failed to initialize Glad!");

		W_CORE_INFO("OpenGL Info:");
		W_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		W_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		W_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		//W_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		//HZ_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}