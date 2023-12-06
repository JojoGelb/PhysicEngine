#include "VisualGameObject.h"
#include "GraphicsMotor.h"
#include <iostream>

unsigned int VisualGameObject::currentId = 0;

glm::mat4 TransformComponent::Mat4()
{
    if (!localCalculation)
        return ExternalMat4;
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
        {translation.x, translation.y, translation.z, 1.0f}};
}

glm::mat3 TransformComponent::NormalMatrix()
{
    if (!localCalculation)
        return ExternalNormalMatrix;
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

VisualGameObject *VisualGameObject::CreatePtrVisualGameObject(std::string modelePath)
{
    VisualGameObject *visual = new VisualGameObject(currentId++);

    Model *lveModel = Model::CreateModelFromFile(GraphicsMotor::GetInstance()->GetVulkanHandler().GetGraphicDevice(), modelePath);
    visual->model = lveModel;
    visual->transformVisual.translation = {0.0f, .0f, 0.0f};
    visual->transformVisual.scale = {1.f, 1.f, 1.f};
    visual->transformVisual.rotation = {0.0f, .0f, 0.0f};
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
    if (transform != nullptr)
    {
        if (!transformVisual.localCalculation)
        {
            // transformVisual.localCalculation = false;
            transformVisual.ExternalMat4 = glm::mat4{
                {
                    transformVisual.scale.x * transform->transformMatrix.values[0],
                    transformVisual.scale.x * transform->transformMatrix.values[1],
                    transformVisual.scale.x * transform->transformMatrix.values[2],
                    0.0f,
                },
                {
                    transformVisual.scale.y * transform->transformMatrix.values[4],
                    transformVisual.scale.y * transform->transformMatrix.values[5],
                    transformVisual.scale.y * transform->transformMatrix.values[6],
                    0.0f,

                },
                {
                    transformVisual.scale.z * transform->transformMatrix.values[8],
                    transformVisual.scale.z * transform->transformMatrix.values[9],
                    transformVisual.scale.z * transform->transformMatrix.values[10],
                    0.0f,

                },

                {
                    transform->transformMatrix.values[3],
                    -transform->transformMatrix.values[7],
                    transform->transformMatrix.values[11],
                    1.0f,
                }

            };
            const glm::vec3 invScale = 1.0f / transformVisual.scale;
            transformVisual.ExternalNormalMatrix = glm::mat3{
                {
                    invScale.x * transform->transformMatrix.values[0],
                    invScale.x * transform->transformMatrix.values[1],
                    invScale.x * transform->transformMatrix.values[2],

                },
                {
                    invScale.y * transform->transformMatrix.values[4],
                    invScale.y * transform->transformMatrix.values[5],
                    invScale.y * transform->transformMatrix.values[6],

                },
                {
                    invScale.z * transform->transformMatrix.values[8],
                    invScale.z * transform->transformMatrix.values[9],
                    invScale.z * transform->transformMatrix.values[10],
                }};
        }
        else
        {
            transformVisual.translation = {transform->position.x, -transform->position.y, transform->position.z};
            transformVisual.rotation = {transform->rotation.x, transform->rotation.y, transform->rotation.z};
            // transformVisual.scale = { transform->scale.x, transform->scale.y, transform->scale.z };
        }
    }
}

void VisualGameObject::Shutdown()
{
    GraphicsMotor::GetInstance()->GetVulkanHandler().RemoveGameObject2(this);
    delete model;
    delete pointLight;
}

VisualGameObject *VisualGameObject::makePointLight(float intensity, float radius, glm::vec3 color)
{
    VisualGameObject *gameObj = VisualGameObject::CreatePtrEmptyVisualGameObject();
    gameObj->color = color;
    gameObj->transformVisual.scale.x = radius;
    gameObj->pointLight = new PointLightComponent();
    gameObj->pointLight->lightIntensity = intensity;
    return gameObj;
}
