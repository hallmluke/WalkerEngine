#pragma once
#include <vulkan/vulkan.h>


namespace Walker {

	class VulkanImageUtils {
	public:
		static VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	};
}