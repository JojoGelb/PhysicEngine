#include "GameObject.h"
#include "../MathPhysicEngine/Particle.h"
#include <VisualGameObject.h>
#include "../MathPhysicEngine/Rigidbody/RigidBody.h"
#include <glm/gtc/quaternion.hpp>


GameObject::GameObject(std::string _name, Transform _transform): name(_name), transform(_transform)
{
}

GameObject::~GameObject()
{
	for (Component* comp : components) {
		comp->Shutdown();
		delete comp;
	}
}


void GameObject::Update()
{
	for(Component* comp: components)
	{
		comp->Update();
	}
}

void GameObject::AddComponent(Component* component)
{
	component->gameObject = this;
	component->transform = &transform;

	//Fix: Rigidbody and visual doesn't use the same type of transform matrix. Need conversion done in visual
	if (typeid(*component) == typeid(VisualGameObject)) {
		VisualGameObject* v = dynamic_cast<VisualGameObject*>(component);
		if (GetComponentOfType<RigidBody>() != nullptr) {
			v->transformVisual.localCalculation = false;
		}
		else if (GetComponentOfType<Particle>() != nullptr) {
			v->transformVisual.localCalculation = true;
		}
	}

	components.push_back(component);
	component->Start();
}
