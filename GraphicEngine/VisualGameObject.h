#pragma once

#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
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

class VisualGameObject : public Component{
public:
	using id_t = unsigned int;
	using Map = std::unordered_map<id_t, VisualGameObject*>;

	static VisualGameObject* CreatePtrVisualGameObject(std::string modelePath);

	static VisualGameObject * CreatePtrEmptyVisualGameObject() {
		static id_t currentId = 0;
		return new VisualGameObject(currentId++);
	}

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

private:
	VisualGameObject(id_t objId) : id(objId) {};

	id_t id;


};