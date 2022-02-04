#include "walkerpch.h"
#include "Framebuffer.h"

#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Walker {

	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(spec);
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}