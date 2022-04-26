#pragma once

#include "Renderer/GraphicsContext.h"
#include "VulkanSwapchain.h"
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <optional>
#include <vector>

struct GLFWwindow;

namespace Walker {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

		static VulkanContext& Get() {
			
			return *s_Instance;
		}

		static VkDevice GetDevice() { return Get().m_Device; }
		static VkPhysicalDevice GetPhysicalDevice() { return Get().m_PhysicalDevice; }
		static VmaAllocator GetAllocator() { return Get().m_Allocator; }
		static VkQueue GetGraphicsQueue() { return Get().m_GraphicsQueue; }
		static VkCommandPool GetCommandPool() { return Get().m_CommandPool; }
		static const uint32_t s_FramesInFlight = 2;
		static uint32_t s_CurrentFrame;

	private:
		static VulkanContext* s_Instance;
		
		GLFWwindow* m_WindowHandle;
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkSurfaceKHR m_Surface;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device;
		VmaAllocator m_Allocator;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;
		std::unique_ptr<VulkanSwapchain> m_Swapchain;

		void CreateInstance();
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		
		void CreateLogicalDevice();
		void CreateAllocator();
		void CreateCommandPool();
		void CreateCommandBuffers();
		void CreateSyncObjects();

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
			VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME
		};
	};

}