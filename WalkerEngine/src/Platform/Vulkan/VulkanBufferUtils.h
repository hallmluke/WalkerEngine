#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Walker {

	class VulkanBufferUtils {
	public:
		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& bufferAllocation, VmaAllocationInfo& allocInfo);
		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		static VkCommandBuffer CreateAndBeginCommandBuffer();
		static void EndCommandBufferAndSubmitToGraphicsQueue(VkCommandBuffer commandBuffer);
	};
}