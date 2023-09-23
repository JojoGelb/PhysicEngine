#pragma once

#include <vulkan/vulkan_core.h>
#include <GraphicDevice.h>
#include "Renderer.h"

#include "GameObject.h" 

#include <memory>
#include <vector>


#include "Camera.h"
#include <SimpleRenderSystem.h>
#include <KeyboardInput.h>

class Window;

class VulkanHandler
{
public:
	VulkanHandler(Window & _window);

	~VulkanHandler();

	void Update(float frameTime = 0);
	void Render();

	void Shutdown();

	GraphicDevice & GetGraphicDevice();

private:
	void LoadGameObjects();

	GraphicDevice graphicDevice;
	Renderer renderer;
	std::vector<GameObject> gameObjects;
	Window & window;

	SimpleRenderSystem renderSystem;
	Camera camera;
	KeyboardInput cameraController;
	GameObject viewerObject; 
};

