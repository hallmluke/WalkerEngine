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

		static void BindDefaultFramebuffer()
		{
			s_RendererAPI->BindDefaultFramebuffer();
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

		static void EnableDepthTest()
		{
			s_RendererAPI->EnableDepthTest();
		}

		static void DisableDepthTest() 
		{
			s_RendererAPI->DisableDepthTest();
		}

		static void SetDepthFunction(RendererAPI::DepthFunction func)
		{
			s_RendererAPI->SetDepthFunction(func);
		}

		static void SetStencilFunction(RendererAPI::StencilFunction func, int32_t ref, uint32_t mask)
		{
			s_RendererAPI->SetStencilFunction(func, ref, mask);
		}

		static void SetStencilMask(uint32_t mask) 
		{
			s_RendererAPI->SetStencilMask(mask);
		}

		static void SetStencilOp(RendererAPI::StencilOp fail, RendererAPI::StencilOp depthFail, RendererAPI::StencilOp pass) 
		{
			s_RendererAPI->SetStencilOp(fail, depthFail, pass);
		}
	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};

}