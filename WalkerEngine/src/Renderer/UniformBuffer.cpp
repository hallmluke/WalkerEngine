#include "walkerpch.h"
#include "UniformBuffer.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"

namespace Walker {

	std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLUniformBuffer>(size, binding);
		case RendererAPI::API::Vulkan:  return std::make_shared<VulkanUniformBuffer>(size, binding);
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}