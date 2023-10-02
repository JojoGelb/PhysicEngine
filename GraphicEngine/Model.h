#pragma once

#include "GraphicDevice.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

class Model {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal{};
        glm::vec2 uv{};

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
    
        bool operator==(const Vertex& other) const {
            return position == other.position && 
                color == other.color && 
                normal == other.normal &&
                uv == other.uv;
        }
    };

    struct Builder {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void LoadModel(const std::string& filepath);
    };

    Model(GraphicDevice& device, const Model::Builder& builder);

    Model(GraphicDevice& device, const std::vector<Vertex>& vertices);
    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    static Model* CreateModelFromFile(
        GraphicDevice& device, const std::string& filepath);

    void Bind(VkCommandBuffer commandBuffer);
    void Draw(VkCommandBuffer commandBuffer);

private:
    void CreateVertexBuffers(const std::vector<Vertex>& vertices);
    void CreateIndexBuffers(const std::vector<uint32_t>& indices);
    
    GraphicDevice& graphicDevice;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;

    bool hasIndexBuffer = false;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    uint32_t indexCount;
};