#include "VulkanRendererAPI.h"

namespace Walker {

	void VulkanRendererAPI::Init()
	{
	}
	void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
	}
	void VulkanRendererAPI::SetClearColor(const glm::vec4& color)
	{
	}
	void VulkanRendererAPI::Clear()
	{
	}
	void VulkanRendererAPI::BindDefaultFramebuffer()
	{
	}
	void VulkanRendererAPI::DrawUnindexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
	{
	}
	void VulkanRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
	}
	void VulkanRendererAPI::EnableBackfaceCulling()
	{
	}
	void VulkanRendererAPI::DisableBackfaceCulling()
	{
	}
	void VulkanRendererAPI::EnableDepthTest()
	{
	}
	void VulkanRendererAPI::DisableDepthTest()
	{
	}
	void VulkanRendererAPI::SetDepthFunction(DepthFunction func)
	{
	}
	void VulkanRendererAPI::SetStencilFunction(StencilFunction func, int32_t ref, uint32_t mask)
	{
	}
	void VulkanRendererAPI::SetStencilMask(uint32_t mask)
	{
	}
	void VulkanRendererAPI::SetStencilOp(StencilOp fail, StencilOp depthFail, StencilOp pass)
	{
	}
}