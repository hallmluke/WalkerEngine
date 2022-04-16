#include "walkerpch.h"
#include "VulkanBuffer.h"
#include "VulkanBufferUtils.h"
#include "VulkanContext.h"

namespace Walker {

	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size)
	{
	}
	VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size)
	{
		VkDeviceSize bufferSize = size;

		VkBuffer stagingBuffer;
		VmaAllocation stagingAllocation;
		VmaAllocationInfo stagingAllocationInfo;
		VulkanBufferUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingAllocation, stagingAllocationInfo);
		//createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		//void* data;
		//vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		void* data;
		vmaMapMemory(VulkanContext::GetAllocator(), stagingAllocation, &data);
		memcpy(data, vertices, (size_t)bufferSize);
		//vkUnmapMemory(device, stagingBufferMemory);
		vmaUnmapMemory(VulkanContext::GetAllocator(), stagingAllocation);

		//createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
		VulkanBufferUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_Allocation, m_AllocationInfo);

		//copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
		VulkanBufferUtils::CopyBuffer(stagingBuffer, m_Buffer, bufferSize);

		vmaDestroyBuffer(VulkanContext::GetAllocator(), stagingBuffer, stagingAllocation);
		//vkDestroyBuffer(device, stagingBuffer, nullptr);
		//vkFreeMemory(device, stagingBufferMemory, nullptr);

		/*VkBuffer stagingBuffer;
		VmaAllocation stagingAllocation;
		VmaAllocationInfo stagingAllocationInfo;
		VulkanBufferUtils::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingAllocation, stagingAllocationInfo);

		memcpy(stagingAllocationInfo.pMappedData, pixels, static_cast<size_t>(imageSize));
		vmaUnmapMemory(VulkanContext::GetAllocator(), stagingAllocation); */
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}

	void VulkanVertexBuffer::Bind() const
	{
		// TODO: How does one pass in a command buffer...
		//vkCmdBindVertexBuffers()
	}
	void VulkanVertexBuffer::Unbind() const
	{
	}
	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
	}




	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count)
	{
		VkDeviceSize bufferSize = sizeof(uint32_t) * count;

		VkBuffer stagingBuffer;
		VmaAllocation stagingAllocation;
		VmaAllocationInfo stagingAllocationInfo;
		VulkanBufferUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingAllocation, stagingAllocationInfo);
		//createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		//void* data;
		//vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		void* data;
		vmaMapMemory(VulkanContext::GetAllocator(), stagingAllocation, &data);
		memcpy(data, indices, (size_t)bufferSize);
		//vkUnmapMemory(device, stagingBufferMemory);
		vmaUnmapMemory(VulkanContext::GetAllocator(), stagingAllocation);

		//createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
		VulkanBufferUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_Allocation, m_AllocationInfo);

		//copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
		VulkanBufferUtils::CopyBuffer(stagingBuffer, m_Buffer, bufferSize);

		vmaDestroyBuffer(VulkanContext::GetAllocator(), stagingBuffer, stagingAllocation);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}

	void VulkanIndexBuffer::Bind() const
	{
	}

	void VulkanIndexBuffer::Unbind() const
	{
	}

	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, uint32_t binding)
	{
		VkDeviceSize bufferSize = size;

		VulkanBufferUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer, m_Allocation, m_AllocationInfo);

		m_DescriptorInfo.buffer = m_Buffer;
		m_DescriptorInfo.offset = 0;
		m_DescriptorInfo.range = size;
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
	}

	void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		void* mappedData;
		vmaMapMemory(VulkanContext::GetAllocator(), m_Allocation, &mappedData);
		memcpy(mappedData, data, size);
		vmaUnmapMemory(VulkanContext::GetAllocator(), m_Allocation);
	}


}