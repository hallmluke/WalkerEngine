#pragma once

#include "Renderer/Texture.h"

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Walker {

	class VulkanTexture2D : public Texture2D
	{
	public:
		VulkanTexture2D(uint32_t width, uint32_t height);
		VulkanTexture2D(const std::string& path);
		VulkanTexture2D(const TextureSpecification& spec);
		virtual ~VulkanTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void BindImage(uint32_t slot, TextureAccess access) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }
		virtual void GenerateMipMaps() override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((VulkanTexture2D&)other).m_RendererID;
		}
	private:
		std::string m_Path;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_MipLevels;
		uint32_t m_RendererID;
		VkImage m_Image;
		VmaAllocation m_Allocation;
		VmaAllocationInfo m_AllocationInfo;
		VkImageView m_ImageView;
		VkSampler m_Sampler;
	};

}