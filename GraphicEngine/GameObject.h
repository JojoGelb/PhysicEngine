#pragma once

#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>



struct TransformComponent {
	glm::vec3 translation{};  // (position offset)
	glm::vec3 scale{ 1.f, 1.f, 1.f };
	glm::vec3 rotation;

	glm::mat4 Mat4();

	glm::mat3 NormalMatrix();
};

class GameObject {
public:
	using id_t = unsigned int;

	static GameObject CreateGameObject() {
		static id_t currentId = 0;
		return GameObject{ currentId++ };
	}

	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject(GameObject&&) = default;
	GameObject& operator=(GameObject&&) = default;

	id_t GetId() { return id; }

	std::shared_ptr<Model> model{};
	glm::vec3 color{};
    TransformComponent transform{};

private:
	GameObject(id_t objId) : id{ objId } {}

	id_t id;
};