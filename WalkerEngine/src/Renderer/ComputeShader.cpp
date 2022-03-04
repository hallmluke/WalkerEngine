#include "walkerpch.h"
#include "ComputeShader.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLComputeShader.h"


namespace Walker {

	std::shared_ptr<ComputeShader> ComputeShader::Create(const std::string& name, const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLComputeShader>(name, path);
		}

		//W_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}