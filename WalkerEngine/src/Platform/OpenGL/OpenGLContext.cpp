#include "walkerpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Walker {


#if defined(W_DEBUG)
	static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
	{
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::string sourceString;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             sourceString = "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceString = "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceString = "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceString = "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     sourceString = "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           sourceString = "Source: Other"; break;
		}

		std::string typeString;
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               typeString = "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeString = "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeString = "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         typeString = "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         typeString = "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              typeString = "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          typeString = "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           typeString = "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               typeString = "Type: Other"; break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         W_CORE_ERROR("OpenGL Debug: Source: {0}, Type: {1}, Message: {2}", sourceString, typeString, message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       W_CORE_WARN("OpenGL Debug: Source: {0}, Type: {1}, Message: {2}", sourceString, typeString, message); break;
//		case GL_DEBUG_SEVERITY_LOW:          W_CORE_INFO("OpenGL Debug: Source: {0}, Type: {1}, Message: {2}", sourceString, typeString, message); break;
//		case GL_DEBUG_SEVERITY_NOTIFICATION: W_CORE_TRACE("OpenGL Debug: Source: {0}, Type: {1}, Message: {2}", sourceString, typeString, message); break;
		}
	}
#endif

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		//W_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		//W_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//W_CORE_ASSERT(status, "Failed to initialize Glad!");

		W_CORE_INFO("OpenGL Info:");
		W_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		W_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		W_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#if defined(W_DEBUG)
		int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
#endif
	}

	void OpenGLContext::SwapBuffers()
	{

		glfwSwapBuffers(m_WindowHandle);
	}

}