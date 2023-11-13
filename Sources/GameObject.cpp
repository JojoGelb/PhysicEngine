#include "GameObject.h"
#include "../MathPhysicEngine/Particle.h"
#include <VisualGameObject.h>
#include "../MathPhysicEngine/Rigidbody/RigidBody.h"
#include <glm/gtc/quaternion.hpp>


GameObject::GameObject(std::string _name): name(_name)
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
	//TODO Do something better like a transform component mandatory
	Particle* p = GetComponentOfType<Particle>();
	VisualGameObject * v = GetComponentOfType<VisualGameObject>();

	RigidBody* r = GetComponentOfType<RigidBody>();

	if (p != nullptr && v != nullptr) {
		v->transform.translation.x = p->finalState.position.x;
		v->transform.translation.y = -p->finalState.position.y;
		v->transform.translation.z = p->finalState.position.z;
	}

	if (r != nullptr && v != nullptr) {

		//Ankward code to convert from Quaternion (Physic motor) to Tait Bryan angles Y1 Z2 X3
		//This strange implementation is due to choice made at early stage of the project and progress made through
		//the course progression
		v->transform.localCalculation = false;
		v->transform.ExternalMat4 = glm::mat4{
			{
				v->transform.scale.x * r->transformMatrix.values[0],
				v->transform.scale.x * r->transformMatrix.values[1],
				v->transform.scale.x * r->transformMatrix.values[2],
				0.0f,
			},
			{
				v->transform.scale.y * r->transformMatrix.values[4],
				v->transform.scale.y * r->transformMatrix.values[5],
				v->transform.scale.y * r->transformMatrix.values[6],
				0.0f,

			},
			{
				v->transform.scale.z * r->transformMatrix.values[8],
				v->transform.scale.z * r->transformMatrix.values[9],
				v->transform.scale.z * r->transformMatrix.values[10],
				0.0f,

			},

			{
				r->transformMatrix.values[3],
				r->transformMatrix.values[7],
				r->transformMatrix.values[11],
				1.0f,
			}
		
		};
		const glm::vec3 invScale = 1.0f / v->transform.scale;
		v->transform.ExternalNormalMatrix = glm::mat3{
			{
				invScale.x * r->transformMatrix.values[0],
				invScale.x * r->transformMatrix.values[1],
				invScale.x * r->transformMatrix.values[2],

			},
			{
				invScale.y * r->transformMatrix.values[4],
				invScale.y * r->transformMatrix.values[5],
				invScale.y * r->transformMatrix.values[6],

			},
			{
				invScale.z * r->transformMatrix.values[8],
				invScale.z * r->transformMatrix.values[9],
				invScale.z * r->transformMatrix.values[10],
			}
		};

	}
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
	component->Start();
}
