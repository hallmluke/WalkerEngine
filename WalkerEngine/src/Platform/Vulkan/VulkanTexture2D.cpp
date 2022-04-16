#include "walkerpch.h"
#include "VulkanTexture2D.h"
#include "VulkanBufferUtils.h"
#include "VulkanImageUtils.h"
#include "VulkanContext.h"
#include <stb_image.h>

namespace Walker {

	VulkanTexture2D::VulkanTexture2D(uint32_t width, uint32_t height)
	{
	}

	VulkanTexture2D::VulkanTexture2D(const std::string& path)
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}

		m_IsLoaded = true;

		m_Width = texWidth;
		m_Height = texHeight;

		VkBuffer stagingBuffer;
		VmaAllocation stagingAllocation;
		VmaAllocationInfo stagingAllocationInfo;
		VulkanBufferUtils::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingAllocation, stagingAllocationInfo);
		//VkDeviceMemory stagingBufferMemory;
		//createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		/*void* data;
		vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(device, stagingBufferMemory);*/
		void* data;
		vmaMapMemory(VulkanContext::GetAllocator(), stagingAllocation, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vmaUnmapMemory(VulkanContext::GetAllocator(), stagingAllocation);

		stbi_image_free(pixels);

		//createImage(texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

		VulkanImageUtils::CreateImage(VulkanContext::GetDevice(), m_Width, m_Height, m_MipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Image, m_Allocation);
		VulkanImageUtils::TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_MipLevels);
		VulkanImageUtils::CopyBufferToImage(stagingBuffer, m_Image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		//copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		//transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);

		vmaDestroyBuffer(VulkanContext::GetAllocator(), stagingBuffer, stagingAllocation);
		//vkDestroyBuffer(device, stagingBuffer, nullptr);
		//vkFreeMemory(device, stagingBufferMemory, nullptr);
		GenerateMipMaps();
		CreateImageView();
		CreateSampler();

		m_DescriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_DescriptorInfo.imageView = m_ImageView;
		m_DescriptorInfo.sampler = m_Sampler;
	}

	VulkanTexture2D::VulkanTexture2D(const TextureSpecification& spec)
	{
		
	}
	VulkanTexture2D::~VulkanTexture2D()
	{
	}
	void VulkanTexture2D::SetData(void* data, uint32_t size)
	{
	}
	void VulkanTexture2D::Bind(uint32_t slot) const
	{
	}
	void VulkanTexture2D::BindImage(uint32_t slot, TextureAccess access) const
	{
	}
	void VulkanTexture2D::GenerateMipMaps()
	{
	}
	void VulkanTexture2D::CreateImageView()
	{
		m_ImageView = VulkanImageUtils::CreateImageView(VulkanContext::GetDevice(), m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_MipLevels);
	}
	void VulkanTexture2D::CreateSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(VulkanContext::GetPhysicalDevice(), &properties);

		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0.0f; // Optional
		samplerInfo.maxLod = static_cast<float>(m_MipLevels);
		samplerInfo.mipLodBias = 0.0f; // Optional

		if (vkCreateSampler(VulkanContext::GetDevice(), &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}
}