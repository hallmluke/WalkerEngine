#include "walkerpch.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Walker {

    std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    /*W_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name, vertexPath, fragmentPath, geometryPath);
		}

		//HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }

}