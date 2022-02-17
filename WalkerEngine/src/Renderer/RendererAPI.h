#pragma once

#include <glm/glm.hpp>

#include "Renderer/VertexArray.h"

namespace Walker {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		enum class DepthFunction
		{
			None = 0,
			LESS,
			LEQUAL
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void BindDefaultFramebuffer() = 0;

		virtual void DrawUnindexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount = 0) = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		virtual void EnableBackfaceCulling() = 0;
		virtual void DisableBackfaceCulling() = 0;

		virtual void SetDepthFunction(DepthFunction func) = 0;

		static API GetAPI() { return s_API; }
		static std::unique_ptr<RendererAPI> Create();
	private:
		static API s_API;
	};

}