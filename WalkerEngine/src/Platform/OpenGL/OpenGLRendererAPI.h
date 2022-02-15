#pragma once

#include "Renderer/RendererAPI.h"

namespace Walker {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void BindDefaultFramebuffer() override;

		virtual void DrawUnindexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount = 0) override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

		virtual void EnableBackfaceCulling() override;
		virtual void DisableBackfaceCulling() override;

		virtual void SetDepthFunction(DepthFunction func) override;
	};


}