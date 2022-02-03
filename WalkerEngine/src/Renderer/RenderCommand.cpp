#include "walkerpch.h"
#include "Renderer/RenderCommand.h"

namespace Walker {

	std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}