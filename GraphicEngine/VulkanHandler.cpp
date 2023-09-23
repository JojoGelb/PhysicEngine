#include "VulkanHandler.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

VulkanHandler::VulkanHandler(Window& _window) :
	graphicDevice(_window),
	renderer(_window, graphicDevice),
	window(_window),
	renderSystem(graphicDevice, renderer.GetSwapChainRenderPass()),
	camera(),
	cameraController(),
	viewerObject(GameObject::CreateGameObject())
{
	//camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f,0.1f,1.f));
	//camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

	LoadGameObjects();
}

VulkanHandler::~VulkanHandler()
{

}

void VulkanHandler::Update(float frameTime)
{
	cameraController.MoveInPlaneXZ(window.GetWindow(), frameTime, viewerObject);
	camera.SetViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

	float aspect = renderer.GetAspectRatio();
	//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
	camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

	for (auto& obj : gameObjects) {
		obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
		obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());
	}
}

void VulkanHandler::Render()
{
	if (auto commandBuffer = renderer.BeginFrame()) {
		renderer.BeginSwapChainRenderPass(commandBuffer);
		renderSystem.RenderGameObjects(commandBuffer, gameObjects, camera);
		renderer.EndSwapChainRenderPass(commandBuffer);
		renderer.EndFrame();
	}
}

void VulkanHandler::Shutdown()
{
	vkDeviceWaitIdle(graphicDevice.Device());
}

GraphicDevice& VulkanHandler::GetGraphicDevice()
{
	return graphicDevice;
}

void VulkanHandler::LoadGameObjects()
{
	std::shared_ptr<Model> lveModel = Model::CreateModelFromFile(graphicDevice, "Models/colored_cube.obj");
	auto gameObject = GameObject::CreateGameObject();
	gameObject.model = lveModel;
	gameObject.transform.translation = { .0f, .0f, 2.5f };
	gameObject.transform.scale = { .5f, .5f, .5f };
	gameObjects.push_back(std::move(gameObject));

	std::shared_ptr<Model> lveModel2 = Model::CreateModelFromFile(graphicDevice, "Models/flat_vase.obj");
	gameObject = GameObject::CreateGameObject();
	gameObject.model = lveModel2;
	gameObject.transform.translation = { .0f, 4.0f, 2.5f };
	gameObject.transform.scale = { .5f, .5f, .5f };
	gameObjects.push_back(std::move(gameObject));

	std::shared_ptr<Model> lveModel3 = Model::CreateModelFromFile(graphicDevice, "Models/smooth_vase.obj");
	gameObject = GameObject::CreateGameObject();
	gameObject.model = lveModel3;
	gameObject.transform.translation = { 4.0f, .0f, 2.5f };
	gameObject.transform.scale = { .5f, .5f, .5f };
	gameObjects.push_back(std::move(gameObject));
}

