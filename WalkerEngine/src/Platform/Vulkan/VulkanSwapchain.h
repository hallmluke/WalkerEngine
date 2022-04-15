#pragma once

#include "Renderer/GraphicsContext.h"
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <GLFW/glfw3.h>
#include <optional>

#include "VulkanRenderPass.h"

namespace Walker {

	class VulkanSwapchain
	{
	public:
		VulkanSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, GLFWwindow* windowHandle);

	private:
		GLFWwindow* m_WindowHandle;
		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		std::vector<VkImageView> m_SwapChainImageViews;

		std::unique_ptr<VulkanRenderPass> m_RenderPass;
		std::vector<VkFramebuffer> m_SwapChainFramebuffers;

		VkImage m_ColorImage;
		VmaAllocation m_ColorAllocation;
		VkImageView m_ColorImageView;

		VkImage m_DepthImage;
		VmaAllocation m_DepthAllocation;
		VkImageView m_DepthImageView;


		void CreateSwapChain(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void CreateImageViews(VkDevice device);
		void CreateRenderPass(VkDevice device, VkPhysicalDevice physicalDevice);
		VkFormat FindDepthFormat(VkPhysicalDevice physicalDevice);
		void CreateColorResources(VkDevice device);
		void CreateDepthResources(VkDevice device, VkPhysicalDevice physicalDevice);
		void CreateFramebuffers(VkDevice device);
	};

}