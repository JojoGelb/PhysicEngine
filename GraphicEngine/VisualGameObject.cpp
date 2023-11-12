#include "VisualGameObject.h"
#include "GraphicsMotor.h"
#include <iostream>

glm::mat4 TransformComponent::Mat4() {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    return glm::mat4{
        {
            scale.x * (c1 * c3 + s1 * s2 * s3),
            scale.x * (c2 * s3),
            scale.x * (c1 * s2 * s3 - c3 * s1),
            0.0f,
        },
        {
            scale.y * (c3 * s1 * s2 - c1 * s3),
            scale.y * (c2 * c3),
            scale.y * (c1 * c3 * s2 + s1 * s3),
            0.0f,
        },
        {
            scale.z * (c2 * s1),
            scale.z * (-s2),
            scale.z * (c1 * c2),
            0.0f,
        },
        {translation.x, translation.y, translation.z, 1.0f} };
}

glm::mat3 TransformComponent::NormalMatrix() {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    const glm::vec3 invScale = 1.0f / scale;

    return glm::mat3{
        {
            invScale.x * (c1 * c3 + s1 * s2 * s3),
            invScale.x * (c2 * s3),
            invScale.x * (c1 * s2 * s3 - c3 * s1),
        },
        {
            invScale.y * (c3 * s1 * s2 - c1 * s3),
            invScale.y * (c2 * c3),
            invScale.y * (c1 * c3 * s2 + s1 * s3),
        },
        {
            invScale.z * (c2 * s1),
            invScale.z * (-s2),
            invScale.z * (c1 * c2),
        },
    };
}

VisualGameObject* VisualGameObject::CreatePtrVisualGameObject(std::string modelePath)
{
    static id_t currentId = 0;

    VisualGameObject* visual = new VisualGameObject(currentId++);

    Model* lveModel = Model::CreateModelFromFile(GraphicsMotor::GetInstance()->GetVulkanHandler().GetGraphicDevice(), modelePath);
    visual->model = lveModel;
    visual->transform.translation = { 0.0f, .0f, 0.0f };
    visual->transform.scale = { 1.f, 1.f, 1.f };
    visual->transform.rotation = { 0.0f, .0f, 0.0f };
    return visual;
}

VisualGameObject::~VisualGameObject()
{
    std::cout << "Delete model \n";
}

void VisualGameObject::Start()
{
    GraphicsMotor::GetInstance()->GetVulkanHandler().AddGameObject2(this);
}

void VisualGameObject::Update()
{
}

void VisualGameObject::Shutdown()
{
    GraphicsMotor::GetInstance()->GetVulkanHandler().RemoveGameObject2(this);
    delete model;
}
