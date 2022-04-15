#include "walkerpch.h"
#include "VulkanSwapchain.h"
#include "VulkanPhysicalDeviceUtils.h"
#include "VulkanImageUtils.h"

namespace Walker {

	VulkanSwapchain::VulkanSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
	{
        CreateSwapChain(physicalDevice, device, surface);
        CreateImageViews(device);
        CreateRenderPass(device, physicalDevice);
        CreateGraphicsPipeline(device);
	}

	void VulkanSwapchain::CreateSwapChain(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface)
	{
        SwapChainSupportDetails swapChainSupport = VulkanPhysicalDeviceUtils::QuerySwapChainSupport(physicalDevice, surface);

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = VulkanPhysicalDeviceUtils::FindQueueFamilies(physicalDevice, surface);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(device, m_SwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, m_SwapChain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;
	}
    VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                // GTX 1050TI hates this mailbox mode for some reason
                //return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(m_WindowHandle, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanSwapchain::CreateImageViews(VkDevice device)
    {
        m_SwapChainImageViews.resize(m_SwapChainImages.size());
        for (uint32_t i = 0; i < m_SwapChainImages.size(); i++) {
            m_SwapChainImageViews[i] = VulkanImageUtils::CreateImageView(device, m_SwapChainImages[i], m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
    }
	void VulkanSwapchain::CreateRenderPass(VkDevice device, VkPhysicalDevice physicalDevice)
	{
        m_RenderPass = std::make_unique<VulkanRenderPass>(device, m_SwapChainImageFormat, FindDepthFormat(physicalDevice));
	}

    void VulkanSwapchain::CreateGraphicsPipeline(VkDevice device)
    {
        m_GraphicsPipeline = std::make_unique<VulkanGraphicsPipeline>(device, m_RenderPass, m_SwapChainExtent.width, m_SwapChainExtent.height);
    }

    VkFormat VulkanSwapchain::FindDepthFormat(VkPhysicalDevice physicalDevice)
    {
        return VulkanPhysicalDeviceUtils::FindSupportedFormat(physicalDevice, 
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }
    void VulkanSwapchain::CreateColorResources(VkDevice device)
    {
        VulkanImageUtils::CreateImage(device, m_SwapChainExtent.width, m_SwapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, m_SwapChainImageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_ColorImage, m_ColorAllocation);
        VulkanImageUtils::CreateImageView(device, m_ColorImage, m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void VulkanSwapchain::CreateDepthResources(VkDevice device, VkPhysicalDevice physicalDevice)
    {
        VkFormat depthFormat = FindDepthFormat(physicalDevice);
        VulkanImageUtils::CreateImage(device, m_SwapChainExtent.width, m_SwapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_DepthImage, m_DepthAllocation);
        VulkanImageUtils::CreateImageView(device, m_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void VulkanSwapchain::CreateFramebuffers(VkDevice device)
    {
        m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());
        for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
            std::array<VkImageView, 3> attachments = {
                m_ColorImageView,
                m_DepthImageView,
                m_SwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_RenderPass->GetRenderPass();
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_SwapChainExtent.width;
            framebufferInfo.height = m_SwapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
}