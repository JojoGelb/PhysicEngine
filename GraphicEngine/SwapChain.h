#pragma once

#include "GraphicDevice.h"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <string>
#include <vector>
#include <memory>

class SwapChain {
public:
	//nbr max de command buffers
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	SwapChain(GraphicDevice& deviceRef, VkExtent2D windowExtent);
	SwapChain(GraphicDevice& deviceRef, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);
	~SwapChain();

	SwapChain(const SwapChain&) = delete;
	SwapChain& operator=(const SwapChain&) = delete;

	VkFramebuffer GetFrameBuffer(int index) { return swapChainFramebuffers[index]; }
	VkRenderPass GetRenderPass() { return renderPass; }
	VkImageView GetImageView(int index) { return swapChainImageViews[index]; }
	size_t ImageCount() { return swapChainImages.size(); }
	VkFormat GetSwapChainImageFormat() { return swapChainImageFormat; }
	VkExtent2D GetSwapChainExtent() { return swapChainExtent; }
	uint32_t Width() { return swapChainExtent.width; }
	uint32_t Height() { return swapChainExtent.height; }
	VkSwapchainKHR GetSwapChainKHR() { return swapChain; }

	float ExtentAspectRatio() {
		return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
	}
	VkFormat FindDepthFormat();

	VkResult AcquireNextImage(uint32_t* imageIndex);
	VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

	bool CompareSwapFormats(const SwapChain& swapChain) const {
		return swapChain.swapChainDepthFormat == swapChainDepthFormat &&
			swapChain.swapChainImageFormat == swapChainImageFormat;
	}

private:
	void Init();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateDepthResources();
	void CreateRenderPass();
	void CreateFramebuffers();
	void CreateSyncObjects();

	// Helper functions
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(
		const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	VkFormat swapChainImageFormat;
	VkFormat swapChainDepthFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkFramebuffer> swapChainFramebuffers; //frame buffer objects
	VkRenderPass renderPass;

	//associated color and depth attachments
	std::vector<VkImage> depthImages; 
	std::vector<VkDeviceMemory> depthImageMemorys;
	std::vector<VkImageView> depthImageViews;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	GraphicDevice& device;
	VkExtent2D windowExtent;

	VkSwapchainKHR swapChain;
	std::shared_ptr<SwapChain> oldSwapChain;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;
};
