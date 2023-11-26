#pragma once

#include "Camera.h"
#include "GraphicDevice.h"
#include "lve_frame_info.h"
#include "VisualGameObject.h"
#include "VKPipeline.h"

// std
#include <memory>
#include <vector>

class PointLightSystem {
 public:
  PointLightSystem(
      GraphicDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void render(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  GraphicDevice &lveDevice;

  std::unique_ptr<VKPipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
};