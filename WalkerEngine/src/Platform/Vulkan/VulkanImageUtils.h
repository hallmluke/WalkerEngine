#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Walker {

	class VulkanImageUtils {
	public:
		static void CreateImage(VkDevice device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VmaAllocation& allocation);
		static VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	};
}