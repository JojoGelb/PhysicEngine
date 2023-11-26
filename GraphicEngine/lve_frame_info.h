#pragma once

#include "Camera.h"
#include "VisualGameObject.h"

// lib
#include <vulkan/vulkan.h>

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  Camera &camera;
  VkDescriptorSet globalDescriptorSet;
  VisualGameObject::Map &gameObjects;
};