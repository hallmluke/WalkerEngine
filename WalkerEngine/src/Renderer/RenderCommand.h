#pragma once

#include "Renderer/RendererAPI.h"

namespace Walker {

	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		static void DrawUnindexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_RendererAPI->DrawUnindexed(vertexArray, count);
		}

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}

		static void EnableBackfaceCulling()
		{
			s_RendererAPI->EnableBackfaceCulling();
		}

		static void DisableBackfaceCulling()
		{
			s_RendererAPI->DisableBackfaceCulling();
		}

		static void SetDepthFunction(RendererAPI::DepthFunction func)
		{
			s_RendererAPI->SetDepthFunction(func);
		}
	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};

}