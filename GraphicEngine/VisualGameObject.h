#pragma once

#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
#include <iostream>
// std
#include <memory>
#include <unordered_map>

struct TransformComponent {
	glm::vec3 translation{};  // (position offset)
	glm::vec3 scale{ 1.f, 1.f, 1.f };
	glm::vec3 rotation;

	glm::mat4 Mat4();

	glm::mat3 NormalMatrix();

	//Bug Fix Code: Let GameObject Handle conversion from Rigidbody transform matrix to this format
	bool localCalculation = true;
	glm::mat4 ExternalMat4;
	glm::mat3 ExternalNormalMatrix;
};

struct PointLightComponent {
  float lightIntensity = 1.0f;
};

class VisualGameObject : public Component{
public:
	using id_t = unsigned int;
	using Map = std::unordered_map<id_t, VisualGameObject*>;

	static id_t currentId;

	static VisualGameObject* CreatePtrVisualGameObject(std::string modelePath);

	static VisualGameObject * CreatePtrEmptyVisualGameObject() {
		return new VisualGameObject(currentId++);
	}

	static VisualGameObject* makePointLight(float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

	~VisualGameObject();

	VisualGameObject(const VisualGameObject&) = delete;
	VisualGameObject& operator=(const VisualGameObject&) = delete;
	VisualGameObject(VisualGameObject&&) = default;
	VisualGameObject& operator=(VisualGameObject&&) = default;

	// H�rit� via Component
	void Start() override;
	void Update() override;
	void Shutdown() override;

	id_t GetId() { return id; }

	Model* model = nullptr;
	glm::vec3 color{};
    TransformComponent transform{};
	float alpha = 1.0f;

	// Optional pointer components
  //std::shared_ptr<Model> model{};
  	PointLightComponent* pointLight = nullptr;

private:
	VisualGameObject(id_t objId) : id(objId) {};

	id_t id;
};