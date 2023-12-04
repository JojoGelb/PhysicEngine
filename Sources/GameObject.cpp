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

	RigidBody* r = GetComponentOfType<RigidBody>();
	
	for(Component* comp: components)
	{
		comp->Update();
	}
	//TODO Do something better like a transform component mandatory
	Particle* p = GetComponentOfType<Particle>();
	VisualGameObject * v = GetComponentOfType<VisualGameObject>();



	if (p != nullptr && v != nullptr) {
		v->transform.translation.x = (float)p->finalState.position.x;
		v->transform.translation.y = (float)-p->finalState.position.y;
		v->transform.translation.z = (float)p->finalState.position.z;
	}

	if (r != nullptr && v != nullptr) {

		//Ankward code to convert from Quaternion (Physic motor) to Tait Bryan angles Y1 Z2 X3
		//This strange implementation is due to choice made at early stage of the project and progress made through
		//the course progression
		v->transform.localCalculation = false;
		v->transform.ExternalMat4 = glm::mat4{
			{
				v->transform.scale.x * r->finalState.transformMatrix.values[0],
				v->transform.scale.x * r->finalState.transformMatrix.values[1],
				v->transform.scale.x * r->finalState.transformMatrix.values[2],
				0.0f,
			},
			{
				v->transform.scale.y * r->finalState.transformMatrix.values[4],
				v->transform.scale.y * r->finalState.transformMatrix.values[5],
				v->transform.scale.y * r->finalState.transformMatrix.values[6],
				0.0f,

			},
			{
				v->transform.scale.z * r->finalState.transformMatrix.values[8],
				v->transform.scale.z * r->finalState.transformMatrix.values[9],
				v->transform.scale.z * r->finalState.transformMatrix.values[10],
				0.0f,

			},

			{
				r->finalState.transformMatrix.values[3],
				- r->finalState.transformMatrix.values[7],
				r->finalState.transformMatrix.values[11],
				1.0f,
			}
		
		};
		const glm::vec3 invScale = 1.0f / v->transform.scale;
		v->transform.ExternalNormalMatrix = glm::mat3{
			{
				invScale.x * r->finalState.transformMatrix.values[0],
				invScale.x * r->finalState.transformMatrix.values[1],
				invScale.x * r->finalState.transformMatrix.values[2],

			},
			{
				invScale.y * r->finalState.transformMatrix.values[4],
				invScale.y * r->finalState.transformMatrix.values[5],
				invScale.y * r->finalState.transformMatrix.values[6],

			},
			{
				invScale.z * r->finalState.transformMatrix.values[8],
				invScale.z * r->finalState.transformMatrix.values[9],
				invScale.z * r->finalState.transformMatrix.values[10],
			}
		};

	}
}

void GameObject::AddComponent(Component* component)
{
	component->gameObject = this;
	component->transform = &transform;
	components.push_back(component);
	component->Start();
}
