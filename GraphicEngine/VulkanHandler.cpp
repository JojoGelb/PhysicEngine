#include "VulkanHandler.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <iostream>

VulkanHandler::VulkanHandler(Window& _window) :
	graphicDevice(_window),
	renderer(_window, graphicDevice),
	window(_window),
	renderSystem(graphicDevice, renderer.GetSwapChainRenderPass()),
	camera(),
	cameraController(),
	viewerObject(VisualGameObject::CreatePtrEmptyVisualGameObject())
{
    viewerObject->transform.translation.z = -5.0f;
    InitImGui();
}

VulkanHandler::~VulkanHandler()
{

}

void VulkanHandler::Update(float frameTime)
{
	cameraController.MoveInPlaneXZ(window.GetWindow(), frameTime, *viewerObject);
	camera.SetViewYXZ(viewerObject->transform.translation, viewerObject->transform.rotation);

	float aspect = renderer.GetAspectRatio();
	//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
	camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
}

void VulkanHandler::Render()
{
	if (auto commandBuffer = renderer.BeginFrame()) {
		renderer.BeginSwapChainRenderPass(commandBuffer);
		//renderSystem.RenderGameObjects(commandBuffer, gameObjects, camera);

        renderSystem.RenderGameObjectsV2(commandBuffer, objects2, camera);
        renderSystem.RenderImGui(commandBuffer);

		renderer.EndSwapChainRenderPass(commandBuffer);
		renderer.EndFrame();
	}
}

void VulkanHandler::Shutdown()
{
    delete viewerObject;
    vkDeviceWaitIdle(graphicDevice.Device());
    ShutdownImGui();

}

GraphicDevice& VulkanHandler::GetGraphicDevice()
{
	return graphicDevice;
}

void VulkanHandler::InitImGui()
{
    VkDescriptorPoolSize pool_sizes[] =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000;
    pool_info.poolSizeCount = std::size(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;

    
    check_vk_result(vkCreateDescriptorPool(graphicDevice.Device(), &pool_info, nullptr, &imguiPool));


    // 2: initialize imgui library

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // 
    //this initializes imgui for SDL
    ImGui_ImplGlfw_InitForVulkan(window.GetWindow(), true);

    //this initializes imgui for Vulkan
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = graphicDevice.GetInstance();
    init_info.PhysicalDevice = graphicDevice.GetPhysicalDevice();
    init_info.Device = graphicDevice.Device();
    init_info.Queue = graphicDevice.GraphicsQueue();
    init_info.DescriptorPool = imguiPool;
    init_info.MinImageCount = 3;
    init_info.ImageCount = 3;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.CheckVkResultFn = check_vk_result;

    ImGui_ImplVulkan_Init(&init_info, renderer.GetSwapChainRenderPass());

    VkResult err = vkResetCommandPool(graphicDevice.Device(), graphicDevice.GetCommandPool(), 0);
    check_vk_result(err);
    auto commandBuffer = graphicDevice.BeginSingleTimeCommands();
    
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

    graphicDevice.EndSingleTimeCommands(commandBuffer);

    //clear font textures from cpu data
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void VulkanHandler::ShutdownImGui()
{
    vkDestroyDescriptorPool(graphicDevice.Device(), imguiPool, nullptr);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void VulkanHandler::check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

