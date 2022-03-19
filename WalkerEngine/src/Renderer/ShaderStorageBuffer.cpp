#include "walkerpch.h"
#include "ShaderStorageBuffer.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShaderStorageBuffer.h"

namespace Walker {
	std::shared_ptr<ShaderStorageBuffer> ShaderStorageBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShaderStorageBuffer>(size, binding);
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}