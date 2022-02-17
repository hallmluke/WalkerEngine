#include "walkerpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Walker {

	/*void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         W_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       W_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          W_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: W_CORE_TRACE(message); return;
		}

		W_CORE_ASSERT(false, "Unknown severity level!");
	}*/

	void OpenGLRendererAPI::Init()
	{
		//W_PROFILE_FUNCTION();

#ifdef HZ_DEBUG
		/*glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);*/
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::BindDefaultFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRendererAPI::DrawUnindexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::EnableBackfaceCulling()
	{
		glEnable(GL_CULL_FACE);
	}

	void OpenGLRendererAPI::DisableBackfaceCulling()
	{
		glDisable(GL_CULL_FACE);
	}

	void OpenGLRendererAPI::SetDepthFunction(DepthFunction func)
	{
		switch (func)
		{
		case DepthFunction::LESS:
			glDepthFunc(GL_LESS);
			break;
		case DepthFunction::LEQUAL:
			glDepthFunc(GL_LEQUAL);
			break;

		}
	}

}