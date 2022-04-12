#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Walker {

	class VulkanRenderPass {
	public:
		VulkanRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat);
		VkRenderPass GetRenderPass() { return m_RenderPass; }

	private:
		void CreateRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat);
		VkRenderPass m_RenderPass;
	};
}