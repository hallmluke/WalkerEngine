#include "walkerpch.h"
#include "Buffer.h"

#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"

namespace Walker {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); */ return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(size);
		case RendererAPI::API::Vulkan:  return std::make_shared<VulkanVertexBuffer>(size);
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		case RendererAPI::API::Vulkan:  return std::make_shared<VulkanVertexBuffer>(vertices, size);
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, size);
		case RendererAPI::API::Vulkan:  return std::make_shared<VulkanIndexBuffer>(indices, size);
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}