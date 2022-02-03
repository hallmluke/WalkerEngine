#include "walkerpch.h"
#include "Buffer.h"

#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Walker {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); */ return nullptr;
		case RendererAPI::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(size);;
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
		case RendererAPI::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);;
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
		case RendererAPI::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}