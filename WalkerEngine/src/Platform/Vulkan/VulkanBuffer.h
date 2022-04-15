#pragma once

#include "Renderer/Buffer.h"
#include "Renderer/UniformBuffer.h"
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Walker {

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(uint32_t size);
		VulkanVertexBuffer(float* vertices, uint32_t size);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;
		VmaAllocationInfo m_AllocationInfo;
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }
	private:
		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;
		VmaAllocationInfo m_AllocationInfo;
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~VulkanUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
		VkDescriptorBufferInfo GetDescriptorInfo() const { return m_DescriptorInfo; }
	private:
		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;
		VmaAllocationInfo m_AllocationInfo;
		VkDescriptorBufferInfo m_DescriptorInfo;
		uint32_t m_RendererID = 0;
	};

}